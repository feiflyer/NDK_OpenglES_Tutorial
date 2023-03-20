package com.fly.opengles.learn;


import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.EBOOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/KM0GEUYA8cwsf5ePXrO8kA
 *
 *
 */
public class EBOActivity extends BaseGlActivity {

    @Override
    public BaseOpengl createOpengl() {
        return new EBOOpengl();
    }
}