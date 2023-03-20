package com.fly.opengles.learn;


import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.Nullable;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.YUVRenderOpengl;
import com.fly.opengles.learn.utils.FileUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/mrPgqOYYElR2A-fLdZ2JFQ
 *
 */

public class YUVRenderActivity extends BaseGlActivity {

    // 注意改成你自己图片的宽高
    private int yuvWidth = 640;
    private int yuvHeight = 428;

    private String nv21Path;
    private String nv12Path;
    private Handler handler = new Handler(Looper.getMainLooper());

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 注意申请磁盘写权限
        // 拷贝资源
        nv21Path = getFilesDir().getAbsolutePath() + "/nv21.yuv";
        FileUtils.copyAssertToDest(this,"nv21.yuv",nv21Path);

        nv12Path = getFilesDir().getAbsolutePath() + "/nv12.yuv";
        FileUtils.copyAssertToDest(this,"nv12.yuv",nv12Path);
    }

    @Override
    public BaseOpengl createOpengl() {
        YUVRenderOpengl yuvRenderOpengl = new YUVRenderOpengl();
        return yuvRenderOpengl;
    }

    @Override
    protected void onResume() {
        super.onResume();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                // 注意nv12和nv21的偏远着色器有点不一样的，需要手动改下调试  YUVRenderOpengl.cpp
//                if(!TextUtils.isEmpty(nv12Path)){
//                    loadYuv(nv12Path,BaseOpengl.YUV_DATA_TYPE_NV12);
//                }

                if(!TextUtils.isEmpty(nv21Path)){
                    loadYuv(nv21Path,BaseOpengl.YUV_DATA_TYPE_NV21);
                }
            }
        },200);
    }

    @Override
    protected void onStop() {
        handler.removeCallbacksAndMessages(null);
        super.onStop();
    }

    private void loadYuv(String path,int yuvType){
        try {
            InputStream inputStream = new FileInputStream(new File(path));
            Log.v("fly_learn_opengl","---length:" + inputStream.available());
            byte[] yData = new byte[yuvWidth * yuvHeight];
            inputStream.read(yData,0,yData.length);
            byte[] uvData = new byte[yuvWidth * yuvHeight / 2];
            inputStream.read(uvData,0,uvData.length);
            Log.v("fly_learn_opengl","---read:" + (yData.length + uvData.length) + "available:" + inputStream.available());
            myGLSurfaceView.setYuvData(yData,uvData,yuvWidth,yuvHeight);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}