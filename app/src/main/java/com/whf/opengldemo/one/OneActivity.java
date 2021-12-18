package com.whf.opengldemo.one;


import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.whf.opengldemo.R;

/**
 * OpenGL基本使用
 */
public class OneActivity extends Activity {

    private GLSurfaceView glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_one);
        glSurfaceView = findViewById(R.id.root_surface);
        initGlSurface();
    }

    private void initGlSurface(){
        //设置GLES版本
        glSurfaceView.setEGLContextClientVersion(3);

        //设置Render
        glSurfaceView.setRenderer(new OneRenderer(this));

        //设置GlSurfaceView渲染模式，一定要在setRenderer之后调用
        //RENDERMODE_WHEN_DIRTY 只有在调用requestRender或者onResume等方法时才渲染
        //RENDERMODE_CONTINUOUSLY 表示一直渲染
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    @Override
    protected void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }
}