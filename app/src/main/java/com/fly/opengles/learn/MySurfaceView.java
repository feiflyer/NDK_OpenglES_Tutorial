package com.fly.opengles.learn;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

import com.fly.opengles.learn.egl.EGLHelper;

public class MySurfaceView extends SurfaceView implements SurfaceHolder.Callback {

    private EGLHelper eglHelper;

    public MySurfaceView(Context context) {
        this(context,null);
    }

    public MySurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        eglHelper = new EGLHelper();
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        eglHelper.surfaceCreated(surfaceHolder.getSurface());
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int w, int h) {
        eglHelper.surfaceChanged(w,h);
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
        eglHelper.surfaceDestroyed();
    }
}
