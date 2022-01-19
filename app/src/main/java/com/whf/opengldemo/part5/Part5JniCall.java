package com.whf.opengldemo.part5;

import android.content.res.AssetManager;

public class Part5JniCall {
    static {
        System.loadLibrary("lib_part5");
    }

    public native void onSurfaceCreated(AssetManager assetManager);

    public native void onSurfaceChanged(int width, int height);

    public native void onDrawFrame();
}
