package com.fly.opengles.learn;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.Matrix;
import android.os.Bundle;
import android.view.View;

import androidx.annotation.Nullable;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.MatrixTransformOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/Z5V7W6bwbamCYMfmFvB2LA
 *
 * https://mp.weixin.qq.com/s/e49kHBlhmpDNtOYG46KJgA
 *
 */
public class MvpMatrixActivity extends BaseGlActivity {

    private MatrixTransformOpengl matrixTransformOpengl;
    // 遵守先缩放再旋转最后平移的顺序
    private final float[] mvpMatrix = new float[16];
    private final float[] mProjectMatrix = new float[16];
    private final float[] modelMatrix = new float[16];
    // 因为在Opengl在顶点坐标的值是在-1到1之间，因此translateX的范围可以为-2到2。
    private float translateX = 0;
    private float scaleX = 1;
    private float rotationZ = 0;

    private int imageWidth;
    private int imageHeight;
    // 是否使用正交投影
    private boolean isUseProjectMatrix = false;

    @Override
    public int getLayoutId() {
        return R.layout.activity_gl_mvp_matrix;
    }

    @Override
    public BaseOpengl createOpengl() {
        matrixTransformOpengl = new MatrixTransformOpengl();
        return matrixTransformOpengl;
    }

    @Override
    public Bitmap requestBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
//      Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic_test, options);
//      Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic_big, options);
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic_boy, options);
        // 设置一下矩阵
        Matrix.setIdentityM(mvpMatrix, 0);
        matrixTransformOpengl.setMvpMatrix(mvpMatrix);
        imageWidth = bitmap.getWidth();
        imageHeight = bitmap.getHeight();
        return bitmap;
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Matrix.setIdentityM(modelMatrix,0);
//        Matrix.setIdentityM(viewMatrix,0);
        Matrix.setIdentityM(mProjectMatrix,0);

//        Matrix.setLookAtM(viewMatrix, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);

        findViewById(R.id.bt_translate).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (null != matrixTransformOpengl) {
                    translateX += 0.1;
                    if (translateX >= 2) {
                        translateX = 0f;
                    }
                    updateMatrix();
                }
            }
        });

        findViewById(R.id.bt_scale).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (null != matrixTransformOpengl) {
                    scaleX += 0.1;
                    updateMatrix();
                }
            }
        });

        findViewById(R.id.bt_rotate).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (null != matrixTransformOpengl) {
                    rotationZ += 10;
                    updateMatrix();
                }
            }
        });

        findViewById(R.id.bt_reset).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (null != matrixTransformOpengl) {
                    translateX = 0;
                    scaleX = 1;
                    rotationZ = 0;
                    isUseProjectMatrix = false;
                    updateMatrix();
                }
            }
        });

        findViewById(R.id.bt_mvp).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (null != matrixTransformOpengl) {
                    isUseProjectMatrix = true;
                    updateMatrix();
                }
            }
        });

    }

    private void calculateProjectMatrix(){
        Matrix.setIdentityM(mProjectMatrix, 0);
        int width = myGLSurfaceView.surfaceWidth;
        int height = myGLSurfaceView.surfaceHeight;
        if(width == 0 || height == 0){
            return;
        }
        // 投影矩阵变换
        // 例如left为-3，right为1，表示整个屏幕是4等分，但是在opengl的顶点坐标中是-1到1也就是占了屏幕的两分，因此如下这个效果是x轴半屏
//                Matrix.orthoM(mvpMatrix, 0, -3, 1, -1, 1, -1, 1);
        float widthScale = (float) width / imageWidth;
        float heightScale = (float) height / imageHeight;
        // 那个缩放小就以那个为基准
        if (heightScale > widthScale) {
            // 图片高度缩放，也就是以宽度为基准
            float newImageHeight = widthScale * imageHeight;
            float r = (float) height / newImageHeight;
            Matrix.orthoM(mProjectMatrix, 0, -1, 1, -r, r, -1, 1);
        } else {
            // 图片宽度缩放
            float newImageWidth = heightScale * imageWidth;
            float r = (float) width / newImageWidth;
            Matrix.orthoM(mProjectMatrix, 0, -r, r, -1, 1, -1, 1);
        }
    }

    private void updateMatrix() {

        if(isUseProjectMatrix){
            calculateProjectMatrix();
        }else {
            Matrix.setIdentityM(mProjectMatrix, 0);
        }

        Matrix.setIdentityM(modelMatrix, 0);
        Matrix.setIdentityM(mvpMatrix, 0);
        // 重点注释
        // 在组合矩阵时，先进行缩放操作，然后是旋转，最后才是位移，但是写法需要反正写，也就是先写translateM，然后rotateM，最后scaleM
        // 如果不这样写会发生什么呢？例如顺这写，先写scaleM，然后是rotateM，最后写translateM，测试时就会出现问题，旋转超过180度之后再移动，就会出现移动方向相反的情况
        Matrix.translateM(modelMatrix, 0, translateX, 0, 0);
        Matrix.rotateM(modelMatrix, 0, rotationZ, 0, 0, 1);
        Matrix.scaleM(modelMatrix, 0, scaleX, 1f, 0f);
        // a 表示旋转角度
        // 后面三个参数，那个参数为1则表示绕那个轴旋转
        Matrix.multiplyMM(mvpMatrix,0,mProjectMatrix,0,modelMatrix,0);
        matrixTransformOpengl.setMvpMatrix(mvpMatrix);
        myGLSurfaceView.requestRender();
    }

}