package com.whf.opengldemo.utils;

import android.content.Context;

import java.io.IOException;
import java.io.InputStream;

public class AssetUtil {

    public static String getGlslSource(Context context, String fileName) {

        InputStream inputStream = null;
        byte[] source = null;

        try {
            inputStream = context.getAssets().open("glsl/" + fileName+".glsl");
            int byteLength = inputStream.available();
            source = new byte[byteLength];
            inputStream.read(source);
        } catch (Exception e) {
            e.printStackTrace();
        }finally {
            if (inputStream!=null){
                try {
                    inputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        return new String(source);
    }
}
