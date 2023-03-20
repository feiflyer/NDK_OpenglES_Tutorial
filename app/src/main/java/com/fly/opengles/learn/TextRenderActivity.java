package com.fly.opengles.learn;

import android.os.Bundle;
import android.util.Log;

import androidx.annotation.Nullable;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.TextRenderOpengl;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class TextRenderActivity extends BaseGlActivity {

    private static final String ttfFileName = "jianti.ttf";

    @Override
    public BaseOpengl createOpengl() {
        TextRenderOpengl textRenderOpengl = new TextRenderOpengl();
        return textRenderOpengl;
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        File file = getTTFFile();
        if (!file.exists() || file.isFile()) {
            // 拷贝字体资源
            BufferedReader bufReader = null;
            try {
                InputStream is = getAssets().open(ttfFileName);
                FileOutputStream fos = new FileOutputStream(file);
                byte[] buffer = new byte[1024];
                int byteCount = 0;
                while ((byteCount = is.read(buffer)) != -1) {
                    // buffer字节
                    fos.write(buffer, 0, byteCount);// 将读取的输入流写入到输出流
                }
                fos.flush();
                is.close();
                fos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }finally {
                // todo 释放
            }
        }
    }

    private File getTTFFile() {
        return new File(getFilesDir(), ttfFileName);
    }
}