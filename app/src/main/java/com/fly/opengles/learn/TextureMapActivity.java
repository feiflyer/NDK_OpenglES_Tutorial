package com.fly.opengles.learn;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.TextureMapOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/jHcR4zzUa4uDw4DV7JlwRQ
 *
 */
public class TextureMapActivity extends BaseGlActivity {

    @Override
    public BaseOpengl createOpengl() {
        TextureMapOpengl textureMapOpengl =  new TextureMapOpengl();
        return textureMapOpengl;
    }

    @Override
    public Bitmap requestBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_boy,options);
        return bitmap;
    }
}