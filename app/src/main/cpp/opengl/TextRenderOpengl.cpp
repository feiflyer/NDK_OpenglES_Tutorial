//
// Created by Fly on 2023/2/21.
//

#include "TextRenderOpengl.h"
#include "ft2build.h"
#include <freetype/ftglyph.h>
#include "utils/Log.h"

// 顶点着色器
static const char *ver = "#version 300 es\n"
                         "in vec4 aPosition;\n"
                         "in vec2 aTexCoord;\n"
                         "out vec2 TexCoord;\n"
                         "void main() {\n"
                         "  TexCoord = aTexCoord;\n"
                         "  gl_Position = aPosition;\n"
                         "}";

// 片元着色器
static const char *fragment = "#version 300 es\n"
                              "precision mediump float;\n"
                              "out vec4 FragColor;\n"
                              "in vec2 TexCoord;\n"
                              "uniform sampler2D ourTexture;\n"
                              "uniform vec3 u_textColor;\n"
                              "void main()\n"
                              "{\n"
                              "    vec4 color = vec4(1.0, 1.0, 1.0, texture(ourTexture, TexCoord).r);\n"
                              "    FragColor = vec4(u_textColor, 1.0) * color;\n"
                              "}";


// 贴图纹理坐标（参考手机屏幕坐标系统，原点在左上角）
const static GLfloat TEXTURE_COORD[] = {
        1.0f, 1.0f, // 右下
        1.0f, 0.0f, // 右上
        0.0f, 1.0f, // 左下
        0.0f, 0.0f // 左上
};

//字体路径，写死了
const static char *FONT_FILE_PATH = "/data/user/0/com.fly.opengles.learn/files/jianti.ttf";

static const int MAX_SHORT_VALUE = 65536;

void TextRenderOpengl::LoadFacesByASCII() {
    // FreeType
    FT_Library ft;
    FT_Face face;
    do {
        if (FT_Init_FreeType(&ft)) {
            LOGD("FT_Init_FreeType fail");
            break;
        }
        // FONT_FILE_PATH替换成自己的字体文件路径
        if (FT_New_Face(ft, FONT_FILE_PATH, 0, &face)) {
            LOGD("FT_New_Face fail");
            break;
        }

        // 设置文字的大小，此函数设置了字体面的宽度和高度
        // 将宽度值设为0表示我们要从字体面通过给出的高度中动态计算出字形的宽度。
        FT_Set_Pixel_Sizes(face, 0, 90);
        // 字节对齐
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                LOGD("FT_Load_Char: Failed to load Glyph");
                continue;
            }
            // 纹理
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_LUMINANCE,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_LUMINANCE,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<GLuint>(face->glyph->advance.x)
            };
            m_Characters.insert(std::pair<GLint, Character>(c, character));
        }
        break;
    } while (0);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 字体库释放
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

// 注意你的字体库要支持中文，如果字体库不支持中文的话渲染是没有效果的，拿到的纹理图的宽高都是0
void TextRenderOpengl::LoadFacesByUnicode(const wchar_t *text, int size) {
    // FreeType
    FT_Library ft;
    FT_Face face;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 因为FT_Library 要释放，所以使用do while
    do {
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft)) {
            // todo 提示
            LOGD("LoadFacesByUnicode -----FT_Init_FreeType fail");
            break;
        }

        if (FT_New_Face(ft, FONT_FILE_PATH, 0, &face)) {
            // todo
            LOGD("LoadFacesByUnicode -----FT_New_Face fail");
            break;
        }

        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 96);
        // 指定使用unicode
        FT_Select_Charmap(face, ft_encoding_unicode);
        for (int i = 0; i < size; ++i) {
            //int index =  FT_Get_Char_Index(face,unicodeArr[i]);
            if (FT_Load_Glyph(face, FT_Get_Char_Index(face, text[i]), FT_LOAD_DEFAULT)) {
                continue;
            }

            FT_Glyph glyph;
            FT_Get_Glyph(face->glyph, &glyph);

            //Convert the glyph to a bitmap.
            FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
            FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

            //This reference will make accessing the bitmap easier
            FT_Bitmap &bitmap = bitmap_glyph->bitmap;

            LOGD("LoadFacesByUnicode -----text[i]:%d",text[i]);
            LOGD("LoadFacesByUnicode -----w:%d,h:%d",bitmap.width,bitmap.rows);

            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_LUMINANCE,
                    bitmap.width,
                    bitmap.rows,
                    0,
                    GL_LUMINANCE,
                    GL_UNSIGNED_BYTE,
                    bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<GLuint>((glyph->advance.x / MAX_SHORT_VALUE) << 6)
            };
            m_Characters.insert(std::pair<GLint, Character>(text[i], character));
            LOGD("LoadFacesByUnicode -----success");
        }
        break;
    } while (0);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

TextRenderOpengl::TextRenderOpengl() {

    initGlProgram(ver, fragment);
    positionHandle = glGetAttribLocation(program, "aPosition");
    textureHandle = glGetAttribLocation(program, "aTexCoord");
    textureSampler = glGetUniformLocation(program, "ourTexture");
    u_textColorHandle = glGetUniformLocation(program, "u_textColor");
    LoadFacesByASCII();
}

TextRenderOpengl::~TextRenderOpengl() noexcept {

}

void TextRenderOpengl::onDraw() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //禁用byte-alignment限制 使用一个字节对齐
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 打开混合设置
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 渲染ASCII字符
    RenderASCIIText("Abcdef%h*!@#$", -0.9f, 0.2f, 3, glm::vec3(0.5, 0.5, 0.5),
                    glm::vec2(eglHelper->viewWidth, eglHelper->viewHeight));

    const wchar_t text[] = L"我看看中文的渲染";
    RenderUnicodeText(text,sizeof(text)/sizeof(text[0]),-0.9f,-0.4f,2,glm::vec3(0.5,0.5,0.5),glm::vec2(eglHelper->viewWidth,eglHelper->viewHeight));

    if (nullptr != eglHelper) {
        eglHelper->swapBuffers();
    }
}


void TextRenderOpengl::RenderUnicodeText(const wchar_t* text, int textLen, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, glm::vec2 viewport){

    LoadFacesByUnicode(text,textLen);

    // 以下代码基本和RenderASCIIText一样的，只是字体库加载的方式变了而已

    // 激活合适的渲染状态
    glUseProgram(program);

    // 纹理坐标都是一样的，先赋值纹理坐标
    // 纹理坐标
    glEnableVertexAttribArray(textureHandle);
    glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, TEXTURE_COORD);

    glUniform3f(u_textColorHandle, color.x, color.y, color.z);
    x *= viewport.x;
    y *= viewport.y;
    for (int i = 0; i < textLen; ++i){
        Character ch = m_Characters[text[i]];

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

        // 归一化
        xpos /= viewport.x;
        ypos /= viewport.y;

        // 字体贴图缩放后的宽高
        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        // 宽高也归一化
        w /= viewport.x;
        h /= viewport.y;

        LOGD("TextRenderSample::RenderUnicodeText [xpos,ypos,w,h]=[%f, %f, %f, %f], ch.advance >> 6 = %d",
             xpos, ypos, w, h, ch.advance >> 6);

        // 当前字符的顶点坐标
        GLfloat VERTICES[] = {
                xpos + w, ypos,
                xpos + w, ypos + h,
                xpos, ypos,
                xpos, ypos + h
        };

        /**
         * size 几个数字表示一个点，显示是两个数字表示一个点
         * normalized 是否需要归一化，不用，这里已经归一化了
         * stride 步长，连续顶点之间的间隔，如果顶点直接是连续的，也可填0
         */
        // 启用顶点数据
        glEnableVertexAttribArray(positionHandle);
        glVertexAttribPointer(positionHandle, 2, GL_FLOAT, GL_FALSE, 0, VERTICES);

        // 在方块上绘制字形纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glUniform1i(textureSampler, 0);
        // 绘制方块
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.advance >> 6) * scale; //(2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 禁用顶点
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(textureHandle);
}

/**
 *
 * @param text 需要渲染的文字
 * @param x 归一化坐标中文字渲染x的起点  [-1,1]
 * @param y 归一化坐标中文字渲染y的起点  [-1,1]
 * @param scale 字体缩放  这个也可以在顶点坐标中做统一的变换使用
 * @param color 字体颜色
 * @param viewport 视窗大小
 */
void TextRenderOpengl::RenderASCIIText(std::string text, GLfloat x, GLfloat y, GLfloat scale,
                                       glm::vec3 color, glm::vec2 viewport) {

    // 激活合适的渲染状态
    glUseProgram(program);

    // 纹理坐标都是一样的，先赋值纹理坐标
    // 纹理坐标
    glEnableVertexAttribArray(textureHandle);
    glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, TEXTURE_COORD);

    glUniform3f(u_textColorHandle, color.x, color.y, color.z);
    // 对文本中的所有字符迭代
    std::string::const_iterator c;
    x *= viewport.x;
    y *= viewport.y;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = m_Characters[*c];

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

        // 归一化
        xpos /= viewport.x;
        ypos /= viewport.y;

        // 字体贴图缩放后的宽高
        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        // 宽高也归一化
        w /= viewport.x;
        h /= viewport.y;

        LOGD("TextRenderSample::RenderText [xpos,ypos,w,h]=[%f, %f, %f, %f], ch.advance >> 6 = %d",
             xpos, ypos, w, h, ch.advance >> 6);

        // 当前字符的顶点坐标
        GLfloat VERTICES[] = {
                xpos + w, ypos,
                xpos + w, ypos + h,
                xpos, ypos,
                xpos, ypos + h
        };

        /**
         * size 几个数字表示一个点，显示是两个数字表示一个点
         * normalized 是否需要归一化，不用，这里已经归一化了
         * stride 步长，连续顶点之间的间隔，如果顶点直接是连续的，也可填0
         */
        // 启用顶点数据
        glEnableVertexAttribArray(positionHandle);
        glVertexAttribPointer(positionHandle, 2, GL_FLOAT, GL_FALSE, 0, VERTICES);

        // 在方块上绘制字形纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glUniform1i(textureSampler, 0);
        // 绘制方块
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.advance >> 6) * scale; //(2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 禁用顶点
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(textureHandle);

}
