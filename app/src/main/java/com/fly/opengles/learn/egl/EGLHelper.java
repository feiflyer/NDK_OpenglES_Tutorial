package com.fly.opengles.learn.egl;

import android.view.Surface;

public class EGLHelper {
    public long nativePtr;

    public EGLHelper() {
        nativePtrInit();
    }

    public void surfaceCreated(Surface surface) {
        nativePtrInit();
        n_surfaceCreated(nativePtr,surface);
    }

    public void surfaceChanged(int width, int height) {
        nativePtrInit();
        n_surfaceChanged(nativePtr,width,height);
    }

    public void surfaceDestroyed() {
        if(nativePtr != 0){
            n_surfaceDestroyed(nativePtr);
            nativePtr = 0;
        }
    }

    private void nativePtrInit(){
        if(nativePtr == 0){
            nativePtr = n_nativePtrInit();
        }
    }

    private native long n_nativePtrInit();
    private native void n_surfaceCreated(long nativePtr,Surface surface);
    private native void n_surfaceChanged(long nativePtr,int width, int height);
    private native void n_surfaceDestroyed(long nativePtr);
}
