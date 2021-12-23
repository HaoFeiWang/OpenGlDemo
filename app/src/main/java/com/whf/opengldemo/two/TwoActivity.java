package com.whf.opengldemo.two;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.whf.opengldemo.R;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * OpenGL Native 实现
 */
public class TwoActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_one);
        TwoJniCall jniCall = new TwoJniCall();
        GLSurfaceView glSurfaceView = findViewById(R.id.root_surface);
        //glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        //glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                jniCall.onSurfaceCreated();
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                jniCall.onSurfaceChanged(width, height);
            }

            @Override
            public void onDrawFrame(GL10 gl) {
                jniCall.onDrawFrame();
            }
        });


    }
}
