package com.fly.opengles.learn.opengl;

import android.graphics.Bitmap;
import android.util.Log;
import android.view.Surface;

import com.fly.opengles.learn.egl.EGLHelper;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;

public class BaseOpengl {

    public static final int YUV_DATA_TYPE_NV12 = 0;
    public static final int YUV_DATA_TYPE_NV21 = 1;

    // 三角形
    public static final int DRAW_TYPE_TRIANGLE = 0;
    // 四边形
    public static final int DRAW_TYPE_RECT = 1;
    // 纹理贴图
    public static final int DRAW_TYPE_TEXTURE_MAP = 2;
    // 矩阵变换
    public static final int DRAW_TYPE_MATRIX_TRANSFORM = 3;
    // VBO/VAO
    public static final int DRAW_TYPE_VBO_VAO = 4;
    // EBO
    public static final int DRAW_TYPE_EBO_IBO = 5;
    // FBO
    public static final int DRAW_TYPE_FBO = 6;
    // PBO
    public static final int DRAW_TYPE_PBO = 7;
    // YUV  nv12与nv21渲染
    public static final int DRAW_YUV_RENDER = 8;
    // 将rgb图像转换城nv21
    public static final int DRAW_RGB_TO_YUV = 9;

    // 水印贴图
    public static final int DRAW_WATER_MARK = 10;

    // 文字渲染
    public static final int DRAW_TEXT_RENDER = 11;
    // 2D纹理数组
    public static final int DRAW_TEXT_ARRAY = 12;
    // MRT多目标渲染
    public static final int DRAW_MRT_RENDER = 13;
    // 2D 的LUT
    public static final int DRAW_2D_LUT_RENDER = 14;
    // cube 的LUT
    public static final int DRAW_cube_LUT_RENDER = 15;

    public long glNativePtr;
    protected EGLHelper eglHelper;
    protected int drawType;

    public BaseOpengl(int drawType) {
        this.drawType = drawType;
        this.eglHelper = new EGLHelper();
    }

    public void surfaceCreated(Surface surface) {
        Log.v("fly_learn_opengl","------------surfaceCreated:" + surface);
        eglHelper.surfaceCreated(surface);
    }

    public void surfaceChanged(int width, int height) {
        Log.v("fly_learn_opengl","------------surfaceChanged:" + Thread.currentThread());
        eglHelper.surfaceChanged(width,height);
    }

    public void surfaceDestroyed() {
        Log.v("fly_learn_opengl","------------surfaceDestroyed:" + Thread.currentThread());
        eglHelper.surfaceDestroyed();
    }

    public void release(){
        if(glNativePtr != 0){
            n_free(glNativePtr,drawType);
            glNativePtr = 0;
        }
    }

    public void onGlDraw(){
        Log.v("fly_learn_opengl","------------onDraw:" + Thread.currentThread());
        if(glNativePtr == 0){
            glNativePtr = n_gl_nativeInit(eglHelper.nativePtr,drawType);
        }
        if(glNativePtr != 0){
            n_onGlDraw(glNativePtr,drawType);
        }
    }

    public void setBitmap(Bitmap bitmap){
        if(glNativePtr == 0){
            glNativePtr = n_gl_nativeInit(eglHelper.nativePtr,drawType);
        }
        if(glNativePtr != 0){
            n_setBitmap(glNativePtr,bitmap);
        }
    }


    /**
     * 设置第二张图片，例如纹理数组的使用
     * @param bitmap
     */
    public void setBitmap2(Bitmap bitmap){
        if(glNativePtr == 0){
            glNativePtr = n_gl_nativeInit(eglHelper.nativePtr,drawType);
        }
        if(glNativePtr != 0){
            n_setBitmap2(glNativePtr,bitmap);
        }
    }

    /**
     * 着色器固定写死了 512X512的滤镜图片，注意尺寸
     * @param bitmap
     */
    public void setLutBitmap(Bitmap bitmap){
        if(glNativePtr == 0){
            glNativePtr = n_gl_nativeInit(eglHelper.nativePtr,drawType);
        }
        if(glNativePtr != 0){
            n_setLutBitmap(glNativePtr,bitmap);
        }
    }

    public void setCubeLut(int width, int height, int depth, FloatBuffer buffer){
        if(glNativePtr == 0){
            glNativePtr = n_gl_nativeInit(eglHelper.nativePtr,drawType);
        }
        if(glNativePtr != 0){
            n_setCubeLut(glNativePtr,width,height,depth,buffer);
        }
    }

    public void setWaterBitmap(Bitmap bitmap){
        if(glNativePtr == 0){
            glNativePtr = n_gl_nativeInit(eglHelper.nativePtr,drawType);
        }
        if(glNativePtr != 0){
            n_setWaterBitmap(glNativePtr,bitmap);
        }
    }

    public void setYuvData(byte[] yData,byte[] uvData,int width,int height){
        if(glNativePtr != 0){
            n_setYuvData(glNativePtr,yData,uvData,width,height,drawType);
        }
    }

    public void setMvpMatrix(float[] mvp){
        if(glNativePtr == 0){
            glNativePtr = n_gl_nativeInit(eglHelper.nativePtr,drawType);
        }
        if(glNativePtr != 0){
            n_setMvpMatrix(glNativePtr,mvp);
        }
    }

    public byte[] readPixel(){
        if(glNativePtr != 0){
            return n_readPixel(glNativePtr,drawType);
        }
        return null;
    }

    public byte[] readYUVResult(){
        if(glNativePtr != 0){
            return n_readYUV(glNativePtr,drawType);
        }
        return null;
    }

    // 绘制
    private native void n_onGlDraw(long ptr,int drawType);
    private native void n_setMvpMatrix(long ptr,float[] mvp);
    private native void n_setBitmap(long ptr,Bitmap bitmap);
    private native void n_setBitmap2(long ptr,Bitmap bitmap);
    private native void n_setLutBitmap(long ptr,Bitmap bitmap);
    private native void n_setCubeLut(long ptr, int width, int height, int depth, FloatBuffer buffer);
    private native void n_setWaterBitmap(long ptr,Bitmap bitmap);
    protected native long n_gl_nativeInit(long eglPtr,int drawType);
    private native void n_free(long ptr,int drawType);
    private native byte[] n_readPixel(long ptr,int drawType);
    private native byte[] n_readYUV(long ptr,int drawType);
    private native void n_setYuvData(long ptr,byte[] yData,byte[] uvData,int width,int height,int drawType);

}
