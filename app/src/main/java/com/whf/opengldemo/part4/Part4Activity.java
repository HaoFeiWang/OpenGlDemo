package com.whf.opengldemo.part4;

import android.app.Activity;
import android.opengl.EGL14;
import android.opengl.EGLConfig;
import android.opengl.EGLContext;
import android.opengl.EGLDisplay;
import android.opengl.EGLSurface;
import android.opengl.GLES30;
import android.os.Bundle;
import android.os.SystemClock;
import android.os.Trace;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.whf.opengldemo.Constants;
import com.whf.opengldemo.R;
import com.whf.opengldemo.utils.AssetUtil;
import com.whf.opengldemo.utils.ShaderUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.LinkedList;

/**
 * EGL基本使用
 * <p>
 * 模拟 UIThread 和 RenderThread 的关系，自主上帧逻辑，不跟随Vsync
 */
public class Part4Activity extends Activity {

    private static final String TAG = Constants.COMMON_TAG + Part4Activity.class.getSimpleName();
    private static final int FRAME_TIME = 8;

    private int mWidth;
    private int mHeight;
    private SurfaceView mSurfaceView;

    private boolean mSurfaceCreated;
    private boolean mInitContext;
    private boolean mReleaseContext;

    private EGLDisplay mEglDisplay;
    private EGLContext mEglContext;
    private EGLSurface mEglSurface;
    private EGLConfig mEglConfig;
    private TestRenderThread mTestRenderThread;
    private ProducerFrame mProducerFrame;

    private long lastTime;
    private MotionEvent mLastEvent;
    private final Object renderLock = new Object();
    private final LinkedList<TestRenderNode> mBufferQueue = new LinkedList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_three);
        initSurfaceView();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    private void initSurfaceView() {
        mSurfaceView = findViewById(R.id.surface_view);
        mSurfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                mSurfaceCreated = true;
                mTestRenderThread = new TestRenderThread();
                mTestRenderThread.start();
                mProducerFrame = new ProducerFrame();
                mProducerFrame.start();
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                mWidth = width;
                mHeight = height;
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {

            }
        });
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        mLastEvent = event;
        long curTime = SystemClock.elapsedRealtime();
        long interval = curTime - lastTime;
        if (interval > 16) {

            //随机制造卡顿
            int random = (int) (Math.random() * 100);
            Trace.beginSection("MainThreeSleep");
            if (random % 30 == 0) {
                try {
                    Thread.sleep(40);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Trace.endSection();

            synchronized (renderLock) {
                mBufferQueue.push(new TestRenderNode(event.getRawX(), event.getRawY()));
                renderLock.notifyAll();
            }
            lastTime = curTime;
        }

        return super.onTouchEvent(event);
    }

    private void initEGL(SurfaceView surfaceView) {
        //1. 获取Display
        mEglDisplay = EGL14.eglGetDisplay(EGL14.EGL_DEFAULT_DISPLAY);
        Log.i(TAG, "eglGetDisplay = " + mEglDisplay);
        if (mEglDisplay == EGL14.EGL_NO_DISPLAY) {
            int errorCode = EGL14.eglGetError();
            Log.e(TAG, "eglGetDisplay error = " + errorCode);
        }

        //3. 初始化
        int[] version = new int[2];
        if (!EGL14.eglInitialize(mEglDisplay, version, 0, version, 1)) {
            int errorCode = EGL14.eglGetError();
            Log.e(TAG, "eglInitialize error = " + errorCode);
        }
        Log.i(TAG, "major version = " + version[0] + " minor = " + version[1]);

        //3. 选择适合配置信息的 EGLConfig
        int[] attrArray = new int[]{
                EGL14.EGL_BUFFER_SIZE, 32,
                EGL14.EGL_RED_SIZE, 8,
                EGL14.EGL_BLUE_SIZE, 8,
                EGL14.EGL_GREEN_SIZE, 8,
                EGL14.EGL_ALPHA_SIZE, 8,
                EGL14.EGL_NONE
        };
        //只需要获取一个EGLConfig（符合配置信息的 EGLConfig 可能有多个）
        EGLConfig[] configArray = new EGLConfig[1];
        //符合 EGLConfig 的个数
        int[] configNumArray = new int[1];
        boolean configResult = EGL14.eglChooseConfig(mEglDisplay,
                attrArray, 0,
                configArray, 0, configArray.length,
                configNumArray, 0);

        if (!configResult) {
            int errorCode = EGL14.eglGetError();
            Log.e(TAG, "eglChooseConfig error = " + errorCode);
        }

        Log.i(TAG, "egl config num = " + configNumArray[0]);
        mEglConfig = configArray[0];
        Log.i(TAG, "eglChooseConfig = " + mEglConfig);
        if (mEglConfig == null) {
            Log.e(TAG, "eglConfig is null");
        }

        //4. 获取Surface
        int[] surfaceAttrArray = new int[]{
                EGL14.EGL_NONE,
        };
        mEglSurface = EGL14.eglCreateWindowSurface(mEglDisplay, mEglConfig,
                surfaceView.getHolder(), surfaceAttrArray, 0);
        Log.i(TAG, "eglCreateWindowSurface = " + mEglSurface);
        if (mEglSurface == EGL14.EGL_NO_SURFACE) {
            int errorCode = EGL14.eglGetError();
            Log.e(TAG, "eglCreateWindowSurface error = " + errorCode);
        }

        //5. 获取 EGLContext（和执行线程相关，每个线程获取到的不一样，需要和 OpenGl 的绘制操作在同一个线程 ）
        int[] configAttrArray = new int[]{
                EGL14.EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL14.EGL_NONE,
        };
        mEglContext = EGL14.eglCreateContext(mEglDisplay, mEglConfig,
                EGL14.EGL_NO_CONTEXT, configAttrArray, 0);
        Log.i(TAG, "eglCreateContext = " + mEglContext);
        if (mEglContext == EGL14.EGL_NO_CONTEXT) {
            int errorCode = EGL14.eglGetError();
            Log.e(TAG, "eglCreateContext error = " + errorCode);
        }

        //6. 为当前线程绑定 EglDisplay、EglSurfaceView、EglContext 等（需要和 OpenGl 的绘制操作在同一个线程）
        if (!EGL14.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
            int errorCode = EGL14.eglGetError();
            Log.e(TAG, "eglMakeCurrent error = " + errorCode);
        }

        Log.i(TAG, "init egl finish!");
    }

    /**
     * 初始化着色器程序
     * 需要和绘制操作在同一个线程
     */
    private void initGLES() {
        String vertex = AssetUtil.getGlslSource(this, "one_vertex");
        String fragment = AssetUtil.getGlslSource(this, "one_fragment");
        int shaderProgram = ShaderUtils.createAndLinkProgram(vertex, fragment);

        GLES30.glUseProgram(shaderProgram);
        //启用顶点着色器的 location = 0 的位置属性
        GLES30.glEnableVertexAttribArray(0);
        GLES30.glViewport(0, 0, mWidth, mHeight);
        Log.i(TAG, "init OpenGLES finish!");
    }

    private void releaseEGL() {
        EGL14.eglMakeCurrent(mEglDisplay, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_CONTEXT);
        EGL14.eglDestroySurface(mEglDisplay, mEglSurface);
        EGL14.eglDestroyContext(mEglDisplay, mEglContext);
        EGL14.eglTerminate(mEglDisplay);
    }

    private void drawFrame() {
        TestRenderNode renderNode;
        synchronized (renderLock) {
            renderNode = mBufferQueue.poll();
            while (renderNode == null) {
                Log.i(TAG, "render node is null!");
                try {
                    renderLock.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                renderNode = mBufferQueue.poll();
            }
        }

        Log.i(TAG, "drawFrame x = " + renderNode.x + " y = " + renderNode.y + " bufferSize = " + mBufferQueue.size());

        //随机制造卡顿
        /*Trace.beginSection("RenderThreadSleep");
        int random = (int) (Math.random() * 100);
        if (random % 40 == 0) {
            try {
                Thread.sleep(60);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        Trace.endSection();*/

        long startTime = SystemClock.elapsedRealtime();
        Log.i(TAG, "startTime = " + startTime);

        //清除屏幕
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);
        GLES30.glClearColor(0, 0, 0, 0);

        //设置顶点数据
        GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT,
                false, 0, createBuffer(renderNode));

        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, 3);
        EGL14.eglSwapBuffers(mEglDisplay, mEglSurface);
        Log.i(TAG, "endTime = " + (SystemClock.elapsedRealtime() - startTime));
    }

    //顶点数据
    private FloatBuffer createBuffer(TestRenderNode renderNode) {
        float[] a = transitionPosition(renderNode.x, renderNode.y);
        float[] b = transitionPosition(renderNode.x + 20, renderNode.y + 20);
        float[] c = transitionPosition(renderNode.x - 20, renderNode.y + 20);

        //三个顶点数据，每个顶点数据包含x、y、z三个值
        float[] vertexArray = {
                a[0], a[1], 0.0f,   // top
                b[0], b[1], 0.0f, // bottom left
                c[0], c[1], 0.0f}; // bottom right

        //三个顶点数据，每个顶点数据包含x、y、z三个值
        /*float[] vertexArray = {
                0.0f, 0.5f, 0.0f,   // top
                -0.5f, -0.5f, 0.0f, // bottom left
                0.5f, -0.5f, 0.0f}; // bottom right*/

        //每个值占用4个字节（32位）
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(vertexArray.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer buffer = byteBuffer.asFloatBuffer();
        buffer.put(vertexArray);
        buffer.position(0);
        return buffer;
    }

    private float[] transitionPosition(float x, float y) {
        float[] position = new float[2];

        float halfWidth = mWidth / 2.0f;
        position[0] = (x - halfWidth) / halfWidth;

        float halfHeight = mHeight / 2.0f;
        position[1] = (halfHeight - y) / halfHeight;

        return position;
    }

    private class TestRenderThread extends Thread {

        TestRenderThread() {
            setName("TestRenderThread");
        }

        @Override
        public void run() {
            while (!mReleaseContext) {
                //保证 mSurfaceView 已经创建成功
                if (!mSurfaceCreated || mWidth == 0 || mHeight == 0) {
                    try {
                        Thread.sleep(30);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    continue;
                }

                if (!mInitContext) {
                    initEGL(mSurfaceView);
                    initGLES();
                    mInitContext = true;
                }

                drawFrame();
            }
        }
    }

    private class TestRenderNode {

        float x;
        float y;

        TestRenderNode(float x, float y) {
            this.x = x;
            this.y = y;
        }

    }

    /**
     * 自动刷新
     */
    private class ProducerFrame extends Thread {
        @Override
        public void run() {
            while (true) {
                try {
                    Thread.sleep(FRAME_TIME);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                synchronized (renderLock) {
                    if (mLastEvent != null) {
                        mBufferQueue.push(new TestRenderNode(mLastEvent.getRawX(), mLastEvent.getRawY()));
                    } else {
                        mBufferQueue.push(new TestRenderNode(0, 0));
                    }
                    renderLock.notifyAll();
                }
            }
        }
    }
}
