package com.fly.opengles.learn;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.fly.opengles.learn.opengl.BaseOpengl;

import com.fly.opengles.learn.opengl.Lut2DOpengl;

public class LUT2DActivity extends BaseGlActivity{

    @Override
    public BaseOpengl createOpengl() {
        Lut2DOpengl lut2DOpengl = new Lut2DOpengl();
        return lut2DOpengl;
    }

    @Override
    public Bitmap requestBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_beauty,options);
        return bitmap;
    }


    @Override
    public Bitmap requestLutBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_lut_gray,options);
        return bitmap;
    }
}
