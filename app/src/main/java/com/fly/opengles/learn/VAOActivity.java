package com.fly.opengles.learn;


import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.VBOVAOOpengl;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/DTLgelOISImvd7CxJRkS9A
 *
 *
 */
public class VAOActivity extends BaseGlActivity {
    @Override
    public BaseOpengl createOpengl() {
        VBOVAOOpengl vbovaoOpengl =  new VBOVAOOpengl();
        return vbovaoOpengl;
    }

}