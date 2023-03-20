package com.fly.opengles.learn;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.TextureArrayOpengl;

public class TextureArrayActivity extends BaseGlActivity {

    @Override
    public BaseOpengl createOpengl() {
        TextureArrayOpengl textureArrayOpengl = new TextureArrayOpengl();
        return textureArrayOpengl;
    }

    @Override
    public Bitmap requestBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_boy,options);
        return bitmap;
    }

    @Override
    public Bitmap requestBitmap2() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_beauty,options);
        // 测试下两张尺寸不一样的，比第一张小的可以，但是比第一张大的不行
//        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_smile,options);
        return bitmap;
    }

}