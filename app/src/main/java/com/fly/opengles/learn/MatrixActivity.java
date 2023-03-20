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
public class MatrixActivity extends BaseGlActivity {

    private MatrixTransformOpengl matrixTransformOpengl;
    // 遵守先缩放再旋转最后平移的顺序
    // 首先执行缩放，接着旋转，最后才是平移。这就是矩阵乘法的工作方式。
    private final float[] mvpMatrix = new float[16];
    // 因为在Opengl在顶点坐标的值是在-1到1之间，因此translateX的范围可以为-2到2。
    private float translateX = 0;
    private float scaleX = 1;
    private float rotationZ = 0;

    @Override
    public int getLayoutId() {
        return R.layout.activity_gl_matrix;
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
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic_boy, options);

        // 设置一下矩阵
        Matrix.setIdentityM(mvpMatrix, 0);
        matrixTransformOpengl.setMvpMatrix(mvpMatrix);

        return bitmap;
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        findViewById(R.id.bt_translate).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (null != matrixTransformOpengl) {
                    translateX += 0.1;
                    if(translateX >=2 ){
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
                    updateMatrix();
                }
            }
        });

    }

    private void updateMatrix() {
        Matrix.setIdentityM(mvpMatrix, 0);
        // 重点注释
        // 在组合矩阵时，先进行缩放操作，然后是旋转，最后才是位移，但是写法需要反正写，也就是先写translateM，然后rotateM，最后scaleM
        // 如果不这样写会发生什么呢？例如顺这写，先写scaleM，然后是rotateM，最后写translateM，测试时就会出现问题，旋转超过180度之后再移动，就会出现移动方向相反的情况
        Matrix.translateM(mvpMatrix, 0, translateX, 0, 0);
        Matrix.rotateM(mvpMatrix, 0, rotationZ, 0, 0, 1);
        Matrix.scaleM(mvpMatrix, 0, scaleX, 1f, 0f);
        matrixTransformOpengl.setMvpMatrix(mvpMatrix);
        myGLSurfaceView.requestRender();
    }
}