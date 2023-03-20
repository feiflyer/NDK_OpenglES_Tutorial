package com.fly.opengles.learn;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.RGBtoYUVOpengl;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/QB7jBW8dbtrbZYIgfk8eqQ
 *
 */
public class RGBToYUVActivity extends AppCompatActivity {

    protected MyGLSurfaceView myGLSurfaceView;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rgb_to_yuv);
        myGLSurfaceView = findViewById(R.id.my_gl_surface_view);
        myGLSurfaceView.setOpenGlListener(new MyGLSurfaceView.OnOpenGlListener() {
            @Override
            public BaseOpengl onOpenglCreate() {
                return new RGBtoYUVOpengl();
            }

            @Override
            public Bitmap requestBitmap() {
                BitmapFactory.Options options = new BitmapFactory.Options();
                options.inScaled = false;
                return BitmapFactory.decodeResource(getResources(), R.mipmap.ic_smile, options);
            }

            @Override
            public Bitmap requestBitmap2() {
                return null;
            }

            @Override
            public void readPixelResult(byte[] bytes) {
                if (null != bytes) {

                }
            }

            @Override
            public void readYUVResult(byte[] bytes) {
                if (null != bytes) {
                    String fileName = System.currentTimeMillis() + ".yuv";
                    File fileParent = getFilesDir();
                    if (!fileParent.exists()) {
                        fileParent.mkdirs();
                    }
                    FileOutputStream fos = null;
                    try {
                        File file = new File(fileParent, fileName);
                        fos = new FileOutputStream(file);
                        fos.write(bytes, 0, bytes.length);
                        fos.flush();
                        fos.close();
                        Toast.makeText(RGBToYUVActivity.this, "YUV图片保存成功" + file.getAbsolutePath(), Toast.LENGTH_LONG).show();
                    } catch (Exception e) {
                        Log.v("fly_learn_opengl", "图片保存异常：" + e.getMessage());
                        Toast.makeText(RGBToYUVActivity.this, "YUV图片保存失败", Toast.LENGTH_LONG).show();
                    }
                }
            }

        });

        Button button = findViewById(R.id.bt_rgb_to_yuv);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                myGLSurfaceView.readYuvData();
            }
        });

        ImageView iv_rgb = findViewById(R.id.iv_rgb);
        iv_rgb.setImageResource(R.mipmap.ic_smile);

    }
}