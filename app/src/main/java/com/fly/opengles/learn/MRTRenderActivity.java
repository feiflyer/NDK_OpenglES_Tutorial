package com.fly.opengles.learn;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.MRTRenderOpengl;

public class MRTRenderActivity extends BaseGlActivity {

    @Override
    public BaseOpengl createOpengl() {
        MRTRenderOpengl mrtRenderOpengl = new MRTRenderOpengl();
        return mrtRenderOpengl;
    }

    @Override
    public Bitmap requestBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_beauty,options);
        return bitmap;
    }
}