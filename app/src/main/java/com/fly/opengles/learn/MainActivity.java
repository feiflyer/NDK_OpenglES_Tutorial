package com.fly.opengles.learn;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;

import com.fly.opengles.learn.databinding.ActivityMainBinding;

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/mp/appmsgalbum?__biz=MzI2MDkzMTExMQ==&action=getalbum&album_id=2547163144436023297#wechat_redirect
 *
 */
public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("learn");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        DemoAdapter demoAdapter = new DemoAdapter();
        binding.rvDemo.setAdapter(demoAdapter);

        requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},100);
    }

}