package com.fly.opengles.learn;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.WaterMarkOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/yGRmVMy7mvnj40Zh2WqaeQ
 *
 */
public class WaterMarkActivity extends BaseGlActivity {

    @Override
    public BaseOpengl createOpengl() {
        WaterMarkOpengl waterMarkOpengl = new WaterMarkOpengl();
        return waterMarkOpengl;
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
    public Bitmap requestWaterBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_love,options);
        return bitmap;
    }
}