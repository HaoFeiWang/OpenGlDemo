package com.whf.opengldemo.two;

public class TwoJniCall {

    static {
        System.loadLibrary("two_lib");
    }

    public native void onSurfaceCreated();

    public native void onSurfaceChanged(int width, int height);

    public native void onDrawFrame();
}
