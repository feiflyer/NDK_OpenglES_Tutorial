package com.fly.opengles.learn;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;

import androidx.annotation.Nullable;

import com.fly.opengles.learn.bean.CubeInfo;
import com.fly.opengles.learn.opengl.BaseOpengl;
import com.fly.opengles.learn.opengl.LutCubeOpengl;
import com.fly.opengles.learn.utils.FileUtils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class LUTCubeActivity extends BaseGlActivity{

    private static final int FLOAT_SZ = Float.SIZE / 8;

    @Override
    public BaseOpengl createOpengl() {
        LutCubeOpengl lutCubeOpengl = new LutCubeOpengl();
        return lutCubeOpengl;
    }

    @Override
    public Bitmap requestBitmap() {
        BitmapFactory.Options options = new BitmapFactory.Options();
        // 不缩放
        options.inScaled = false;
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_beauty,options);
        return bitmap;
    }

    @Override
    public CubeInfo requestCubeLut() {
        CubeInfo cubeInfo = null;
        BufferedReader bufReader = null;
        FloatBuffer lutBuffer = null;
        int size = 0;
        try {
            InputStreamReader inputReader = new InputStreamReader(getResources().getAssets().open("3dlut_01.cube"));
            bufReader = new BufferedReader(inputReader);
            String line = "";
            while ((line = bufReader.readLine()) != null){
                if(line.startsWith("LUT_3D_SIZE")){
                    size = Integer.valueOf(line.split(" ")[1]);
                    int allSize = size * size * size * 3;
                    //将位图加载到opengl中，并复制到当前绑定的纹理对象上
                    lutBuffer = ByteBuffer.allocateDirect(allSize * FLOAT_SZ)
                            .order(ByteOrder.nativeOrder()).asFloatBuffer();
                    cubeInfo = new CubeInfo();
                    cubeInfo.setWidth(size);
                    cubeInfo.setHeight(size);
                    cubeInfo.setDepth(size);
                    cubeInfo.setBuffer(lutBuffer);
                }else if(line.startsWith("0") || line.startsWith("1") || line.startsWith("2") || line.startsWith("3") || line.startsWith("4") || line.startsWith("5")
                        || line.startsWith("6") || line.startsWith("7") || line.startsWith("8") || line.startsWith("9")){
                    String[] datas = line.split(" ");
                    for (int i = 0; i < datas.length; i++) {
                        lutBuffer.put(Float.valueOf(datas[i]));
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
            try {
                bufReader.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
            return null;
        }
        if(null != lutBuffer){
            lutBuffer.position(0);
        }
        return cubeInfo;
    }

}
