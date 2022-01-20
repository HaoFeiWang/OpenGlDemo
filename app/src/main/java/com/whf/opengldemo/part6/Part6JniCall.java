package com.whf.opengldemo.part6;

import android.content.res.AssetManager;

public class Part6JniCall {
    static {
        System.loadLibrary("lib_part6");
    }

    public native void onSurfaceCreated(AssetManager assetManager);

    public native void onSurfaceChanged(int width, int height);

    public native void onDrawFrame();

    public native void translation(float x, float y, float z);
}
