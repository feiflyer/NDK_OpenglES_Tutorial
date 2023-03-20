package com.fly.opengles.learn;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.os.Looper;


import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.PBOOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/nHo3yXWdxwijvyLYXvTa7Q
 *
 *
 */
public class PBOActivity extends BaseGlActivity {

    private int index = 1;
    private Handler mainHandler = new Handler(Looper.getMainLooper());
    private Runnable changeBitmapRunnable = new Runnable() {
        @Override
        public void run() {
            index++;
            myGLSurfaceView.updateBitmap();
            mainHandler.postDelayed(this,2000);
        }
    };

    @Override
    protected void onResume() {
        mainHandler.postDelayed(changeBitmapRunnable,2000);
        super.onResume();
    }

    @Override
    protected void onStop() {
        mainHandler.removeCallbacksAndMessages(null);
        super.onStop();
    }

    @Override
    public BaseOpengl createOpengl() {
        PBOOpengl pboOpengl = new PBOOpengl();
        return pboOpengl;
    }

    @Override
    public Bitmap requestBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        if(index % 2 == 0){
            return BitmapFactory.decodeResource(getResources(),R.mipmap.ic_boy,options);
        }else {
            return BitmapFactory.decodeResource(getResources(),R.mipmap.ic_beauty,options);
        }
    }

}