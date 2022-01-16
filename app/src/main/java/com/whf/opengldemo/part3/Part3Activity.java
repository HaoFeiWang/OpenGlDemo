package com.whf.opengldemo.part3;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.whf.opengldemo.R;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Part3Activity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_part_3);
        GLSurfaceView glSurfaceView = findViewById(R.id.root_surface);

        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setRenderer(new GLSurfaceView.Renderer() {
            final Part3JniCall jniCall = new Part3JniCall();

            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                jniCall.onSurfaceCreated(getAssets());
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

        //一定要在setRenderer之后调用
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }
}