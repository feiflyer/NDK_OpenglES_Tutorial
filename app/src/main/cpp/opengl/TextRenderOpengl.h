//
// Created by Fly on 2023/2/21.
//

#ifndef NDK_OPENGLES_LEARN_TEXTRENDEROPENGL_H
#define NDK_OPENGLES_LEARN_TEXTRENDEROPENGL_H

#include "BaseOpengl.h"
#include "glm/glm.hpp"
#include <map>

struct Character {
    GLuint textureID;   // ID handle of the glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
    GLuint advance;    // Horizontal offset to advance to next glyph
};

class TextRenderOpengl:public BaseOpengl  {
public:
    TextRenderOpengl();

    virtual ~TextRenderOpengl();

    // override要么就都写，要么就都不写，不要一个虚函数写override，而另外一个虚函数不写override，不然可能编译不过
    virtual void onDraw() override;

private:
    void LoadFacesByASCII();
    GLint positionHandle{-1};
    GLint textureHandle{-1};
    GLint textureSampler{-1};
    GLuint u_textColorHandle{0};
    std::map<GLint, Character> m_Characters;

    void RenderASCIIText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color,
                    glm::vec2 viewport);
    void RenderUnicodeText(const wchar_t* text, int textLen, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, glm::vec2 viewport);
    void LoadFacesByUnicode(const wchar_t *text, int size);
};


#endif //NDK_OPENGLES_LEARN_TEXTRENDEROPENGL_H
