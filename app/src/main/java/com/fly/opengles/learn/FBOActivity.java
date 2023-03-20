package com.fly.opengles.learn;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.FBOOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/aEcr09LBaALcJlDnAmaCuw
 *
 *
 */
public class FBOActivity extends BaseGlActivity {

    @Override
    public BaseOpengl createOpengl() {
        FBOOpengl fboOpengl = new FBOOpengl();
        return fboOpengl;
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