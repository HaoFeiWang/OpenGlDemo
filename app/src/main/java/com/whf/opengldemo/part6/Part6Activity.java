package com.whf.opengldemo.part6;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.util.Log;
import android.view.View;

import com.whf.opengldemo.Constants;
import com.whf.opengldemo.R;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Part6Activity extends Activity implements View.OnClickListener {

    private static final String TAG = Constants.COMMON_TAG + Part6Activity.class.getSimpleName();

    private Part6JniCall jniCall;
    private Handler mHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_part_6);
        GLSurfaceView glSurfaceView = findViewById(R.id.root_surface);
        jniCall = new Part6JniCall();

        HandlerThread nativeThread = new HandlerThread("Native-Thread");
        nativeThread.start();
        mHandler = new Handler(nativeThread.getLooper());

        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setRenderer(new GLSurfaceView.Renderer() {

            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                /*mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        jniCall.onSurfaceCreated(getAssets());
                    }
                });*/
                jniCall.onSurfaceCreated(getAssets());
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                //Log.i(TAG,"onSurfaceChanged ThreadId = "+Thread.currentThread().getId());
                /*mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        jniCall.onSurfaceChanged(width, height);
                    }
                });*/
                jniCall.onSurfaceChanged(width, height);
            }

            @Override
            public void onDrawFrame(GL10 gl) {
                //Log.i(TAG,"onDrawFrame ThreadId = "+Thread.currentThread().getId());
                /*mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        jniCall.onDrawFrame();
                    }
                });*/
                jniCall.onDrawFrame();
            }
        });

        //一定要在setRenderer之后调用
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);

        findViewById(R.id.btn_translation).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_translation:
                Log.i(TAG, "onClick ThreadId = " + Thread.currentThread().getId());
                /*mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        Log.i(TAG, "onClick ThreadId = " + Thread.currentThread().getId());
                        jniCall.translation(0.5f, 0.5f, 0.5f);
                    }
                });*/
                jniCall.translation(0.5f, 0.5f, 0.5f);
                break;
        }
    }
}