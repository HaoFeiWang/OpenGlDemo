package com.whf.opengldemo.part3;

import android.content.res.AssetManager;

public class Part3JniCall {
    static {
        System.loadLibrary("lib_part3");
    }

    public native void onSurfaceCreated(AssetManager assetManager);

    public native void onSurfaceChanged(int width, int height);

    public native void onDrawFrame();
}
