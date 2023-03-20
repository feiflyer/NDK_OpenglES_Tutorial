package com.fly.opengles.learn;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.util.Size;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

import com.fly.opengles.learn.bean.CubeInfo;
import com.fly.opengles.learn.bean.YUVBean;
import com.fly.opengles.learn.opengl.BaseOpengl;

public class MyGLSurfaceView extends SurfaceView implements SurfaceHolder.Callback {

    private final static int MSG_CREATE_GL = 101;
    private final static int MSG_CHANGE_GL = 102;
    private final static int MSG_DRAW_GL = 103;
    private final static int MSG_DESTROY_GL = 104;
    private final static int MSG_READ_PIXEL_GL = 105;
    private final static int MSG_UPDATE_BITMAP_GL = 106;
    private final static int MSG_UPDATE_YUV_GL = 107;
    private final static int MSG_READ_YUV_GL = 108;

    public BaseOpengl baseOpengl;
    private OnOpenGlListener onOpenGlListener;
    private OnSurfaceSizeChangeListener surfaceSizeChangeListener;
    private HandlerThread handlerThread;
    private Handler renderHandler;

    public int surfaceWidth;
    public int surfaceHeight;

    public MyGLSurfaceView(Context context) {
        this(context,null);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        getHolder().addCallback(this);
        handlerThread = new HandlerThread("RenderHandlerThread");
        handlerThread.start();
        renderHandler = new Handler(handlerThread.getLooper()){
            @Override
            public void handleMessage(@NonNull Message msg) {
                switch (msg.what){
                    case MSG_CREATE_GL:
                        baseOpengl = onOpenGlListener.onOpenglCreate();
                        Surface surface = (Surface) msg.obj;
                        if(null != baseOpengl){
                            baseOpengl.surfaceCreated(surface);
                            Bitmap bitmap = onOpenGlListener.requestBitmap();
                            if(null != bitmap){
                                baseOpengl.setBitmap(bitmap);
                            }

                            Bitmap bitmap2 = onOpenGlListener.requestBitmap2();
                            if(null != bitmap2){
                                baseOpengl.setBitmap2(bitmap2);
                            }

                            Bitmap waterBitmap = onOpenGlListener.requestWaterBitmap();
                            if(null != waterBitmap){
                                baseOpengl.setWaterBitmap(waterBitmap);
                            }

                            Bitmap lutBitmap = onOpenGlListener.requestLutBitmap();
                            if(null != lutBitmap){
                                baseOpengl.setLutBitmap(lutBitmap);
                            }

                            CubeInfo cubeLut = onOpenGlListener.requestCubeLut();
                            if(null != cubeLut){
                                baseOpengl.setCubeLut(cubeLut.getWidth(),cubeLut.getHeight(),cubeLut.getDepth(),cubeLut.getBuffer());
                            }

                        }
                        break;

                    case MSG_CHANGE_GL:
                        if(null != baseOpengl){
                            Size size = (Size) msg.obj;
                            baseOpengl.surfaceChanged(size.getWidth(),size.getHeight());
                        }
                        break;

                    case MSG_DRAW_GL:
                        if(null != baseOpengl){
                            baseOpengl.onGlDraw();
                        }
                        break;

                    case MSG_READ_PIXEL_GL:
                        if(null != baseOpengl){
                           byte[] bytes = baseOpengl.readPixel();
                           if(null != bytes && null != onOpenGlListener){
                               onOpenGlListener.readPixelResult(bytes);
                           }
                        }
                        break;

                    case MSG_READ_YUV_GL:
                        if(null != baseOpengl){
                            byte[] bytes = baseOpengl.readYUVResult();
                            if(null != bytes && null != onOpenGlListener){
                                onOpenGlListener.readYUVResult(bytes);
                            }
                        }
                        break;


                    case MSG_UPDATE_BITMAP_GL:
                        if(null != baseOpengl){
                            Bitmap bitmap = onOpenGlListener.requestBitmap();
                            if(null != bitmap){
                                baseOpengl.setBitmap(bitmap);
                                baseOpengl.onGlDraw();
                            }
                        }
                        break;

                    case MSG_UPDATE_YUV_GL:
                        if(null != baseOpengl){
                            YUVBean yuvBean = (YUVBean) msg.obj;
                            if(null != yuvBean){
                                baseOpengl.setYuvData(yuvBean.getyData(),yuvBean.getUvData(),yuvBean.getWidth(),yuvBean.getHeight());
                                baseOpengl.onGlDraw();
                            }
                        }
                        break;

                    case MSG_DESTROY_GL:
                        if(null != baseOpengl){
                            baseOpengl.surfaceDestroyed();
                        }
                        break;
                }
            }
        };
    }

    public void setOpenGlListener(OnOpenGlListener listener) {
        this.onOpenGlListener = listener;
    }

    public void setSurfaceSizeChangeListener(OnSurfaceSizeChangeListener listener){
        this.surfaceSizeChangeListener = listener;
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        Message message = Message.obtain();
        message.what = MSG_CREATE_GL;
        message.obj = surfaceHolder.getSurface();
        renderHandler.sendMessage(message);
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int w, int h) {
        Message message = Message.obtain();
        message.what = MSG_CHANGE_GL;
        message.obj = new Size(w,h);
        renderHandler.sendMessage(message);

        Message message1 = Message.obtain();
        message1.what = MSG_DRAW_GL;
        renderHandler.sendMessage(message1);

        surfaceWidth = w;
        surfaceHeight = h;

        if(null != surfaceSizeChangeListener){
            surfaceSizeChangeListener.onSurfaceSizeChange(surfaceWidth,surfaceHeight);
        }

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
        Message message = Message.obtain();
        message.what = MSG_DESTROY_GL;
        renderHandler.sendMessage(message);
    }

    public void readGlPixel(){
        Message message = Message.obtain();
        message.what = MSG_READ_PIXEL_GL;
        renderHandler.sendMessage(message);
    }

    public void readYuvData(){
        Message message = Message.obtain();
        message.what = MSG_READ_YUV_GL;
        renderHandler.sendMessage(message);
    }

    public void updateBitmap(){
        Message message = Message.obtain();
        message.what = MSG_UPDATE_BITMAP_GL;
        renderHandler.sendMessage(message);
    }

    public void setYuvData(byte[] yData,byte[] uvData,int width,int height){
        Message message = Message.obtain();
        message.what = MSG_UPDATE_YUV_GL;
        message.obj = new YUVBean(yData,uvData,width,height);
        renderHandler.sendMessage(message);
    }

    public void release(){
        // todo 主要线程同步问题，当心surfaceDestroyed还没有执行到，但是就被release了，那就内存泄漏了
        if(null != baseOpengl){
            baseOpengl.release();
        }
    }

    public void requestRender(){
        Message message = Message.obtain();
        message.what = MSG_DRAW_GL;
        renderHandler.sendMessage(message);
    }

    public interface OnOpenGlListener{
        BaseOpengl onOpenglCreate();
        Bitmap requestBitmap();
        Bitmap requestBitmap2();
        default Bitmap requestWaterBitmap(){
            return null;
        }

        default Bitmap requestLutBitmap(){
            return null;
        }

        default CubeInfo requestCubeLut(){
            return null;
        }

        void readPixelResult(byte[] bytes);
        void readYUVResult(byte[] bytes);
    }

    public interface OnSurfaceSizeChangeListener{
        void onSurfaceSizeChange(int width,int height);
    }

}
