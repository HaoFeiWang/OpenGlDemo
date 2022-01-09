package com.whf.opengldemo.part2;

import android.content.res.AssetManager;

public class Part2JniCall {

    static {
        System.loadLibrary("lib_part2");
    }

    public native void onSurfaceCreated(AssetManager assetManager);

    public native void onSurfaceChanged(int width, int height);

    public native void onDrawFrame();
}
