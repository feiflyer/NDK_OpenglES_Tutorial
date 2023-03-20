package com.fly.opengles.learn;

import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.RectOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/x2u6F1weIeOpPDPq48GbRA
 *
 */
public class DrawRectActivity extends BaseGlActivity{
    @Override
    public BaseOpengl createOpengl() {
        return new RectOpengl();
    }
}
