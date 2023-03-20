package com.fly.opengles.learn;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.fly.opengles.learn.bean.CubeInfo;
import com.fly.opengles.learn.opengl.BaseOpengl;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;

public class BaseGlActivity extends AppCompatActivity {

    protected MyGLSurfaceView myGLSurfaceView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(getLayoutId());
        myGLSurfaceView = findViewById(R.id.my_gl_surface_view);
        myGLSurfaceView.setOpenGlListener(new MyGLSurfaceView.OnOpenGlListener() {
            @Override
            public BaseOpengl onOpenglCreate() {
                return createOpengl();
            }

            @Override
            public Bitmap requestBitmap() {
                return BaseGlActivity.this.requestBitmap();
            }

            @Override
            public Bitmap requestLutBitmap() {
                return BaseGlActivity.this.requestLutBitmap();
            }

            @Override
            public CubeInfo requestCubeLut() {
                return BaseGlActivity.this.requestCubeLut();
            }

            @Override
            public Bitmap requestBitmap2() {
                return BaseGlActivity.this.requestBitmap2();
            }

            @Override
            public Bitmap requestWaterBitmap() {
                return BaseGlActivity.this.requestWaterBitmap();
            }

            @Override
            public void readPixelResult(byte[] bytes) {
                if (null != bytes) {
                    Log.v("fly_learn_opengl", "bytes：" + bytes.length);
                    Bitmap bitmap = Bitmap.createBitmap(myGLSurfaceView.surfaceWidth, myGLSurfaceView.surfaceHeight, Bitmap.Config.ARGB_8888);

                    ByteBuffer buffer = ByteBuffer.allocate(bytes.length);
                    buffer.put(bytes, 0, bytes.length);
                    buffer.position(0);

                    bitmap.copyPixelsFromBuffer(buffer);

                    String fileName = System.currentTimeMillis() + ".jpg";
                    File fileParent = getFilesDir();
                    if (!fileParent.exists()) {
                        fileParent.mkdirs();
                    }
                    FileOutputStream fos = null;
                    try {
                        File file = new File(fileParent, fileName);
                        fos = new FileOutputStream(file);
                        bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fos);
                        fos.flush();
                        fos.close();
                        Toast.makeText(BaseGlActivity.this, "图片保存成功" + file.getAbsolutePath(), Toast.LENGTH_LONG).show();
                    } catch (Exception e) {
                        Log.v("fly_learn_opengl", "图片保存异常：" + e.getMessage());
                        Toast.makeText(BaseGlActivity.this, "图片保存失败", Toast.LENGTH_LONG).show();
                    }
                }
            }

            @Override
            public void readYUVResult(byte[] bytes) {

            }

        });

        Button button = findViewById(R.id.bt_read_gl);
        if(null != button){
            button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    myGLSurfaceView.readGlPixel();
                }
            });
        }
    }

    @Override
    protected void onDestroy() {
        if (null != myGLSurfaceView) {
            myGLSurfaceView.release();
        }
        super.onDestroy();
    }

    public BaseOpengl createOpengl() {
        return null;
    }

    public Bitmap requestBitmap() {
        return null;
    }


    public Bitmap requestBitmap2() {
        return null;
    }

    public Bitmap requestWaterBitmap(){
        return null;
    }

    public Bitmap requestLutBitmap() {
        return null;
    }

    public CubeInfo requestCubeLut(){
        return null;
    }

    public int getLayoutId() {
        return R.layout.activity_base_gl;
    }

}
