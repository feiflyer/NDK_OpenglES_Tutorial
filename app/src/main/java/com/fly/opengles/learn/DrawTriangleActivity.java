package com.fly.opengles.learn;


import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.TriangleOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/vRiKvtrMkt-S1ShHegvW1A
 *
 */
public class DrawTriangleActivity extends BaseGlActivity {

    @Override
    public BaseOpengl createOpengl() {
        return new TriangleOpengl();
    }
}