package com.fly.opengles.learn.utils;

import android.content.Context;
import android.util.Log;


import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

public class FileUtils {
    public static void copyAssertToDest(Context context,String assertPath,String destPath){
        File destFile = new File(destPath);
        File parentFile = destFile.getParentFile();
        if(!parentFile.exists()){
            parentFile.mkdirs();
        }
        try {
            InputStream inputStream = context.getAssets().open(assertPath);
            OutputStream outputStream = new FileOutputStream(destFile);
            byte[] buf = new byte[1024];
            int len;
            while ((len = inputStream.read(buf)) > 0) {
                outputStream.write(buf, 0, len);
            }
            inputStream.close();
            outputStream.close();
            Log.v("aaaaaa","------拷贝成功");
        } catch (Exception e) {
            Log.v("aaaaaa","------拷贝异常");
            e.printStackTrace();
        }
    }
}
