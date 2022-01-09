package com.whf.opengldemo.part2;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.whf.opengldemo.R;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * OpenGL Native 实现
 */
public class Part2Activity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_part_2);
        GLSurfaceView glSurfaceView = findViewById(R.id.root_surface);

        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setRenderer(new GLSurfaceView.Renderer() {
            final Part2JniCall jniCall = new Part2JniCall();

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
