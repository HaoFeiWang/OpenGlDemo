package com.whf.opengldemo.part1;

import static com.whf.opengldemo.Constants.COMMON_TAG;

import android.content.Context;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.util.Log;

import com.whf.opengldemo.utils.AssetUtil;
import com.whf.opengldemo.utils.ShaderUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Part1Renderer implements GLSurfaceView.Renderer {

    private static final String TAG = COMMON_TAG + Part1Renderer.class.getSimpleName();

    private Context mContext;

    private int program;
    //private int vPosition;

    public Part1Renderer(Context context){
        this.mContext = context;
    }

    /**
     * GLSurfaceView创建完成，也代表OpenGL ES环境创建完成，通常情况下在此方法中创建Program及初始化参数
     *
     * @param gl
     * @param config
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.i(TAG, "onSurfaceCreated");
        String vertexSource = AssetUtil.getGlslSource(mContext,"one_vertex");
        String fragmentSource = AssetUtil.getGlslSource(mContext,"one_fragment");

        program = ShaderUtils.createAndLinkProgram(vertexSource,fragmentSource);
        //获取着色器的属性位置（vPosition为顶点着色器的输入属性）
        //vPosition = GLES30.glGetAttribLocation(program,"vPosition");
    }

    /**
     * 当Surface发生变化的时候回调，比如竖屏转横屏导致GLSurfaceView大小发生变化
     * 通常情况下在此方法中设置绘制窗口及和GLSurfaceView大小有关系的参数
     *
     * @param gl
     * @param width
     * @param height
     */
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.i(TAG, "onSurfaceChanged");
        //创建OpenGL ES绘制窗口，此处传入的x，y坐标系原点为屏幕左下角
        GLES30.glViewport(width/2,height/2,width/2,height/2);
    }


    /**
     * 执行OpenGL ES渲染工作，由系统以一定的频率来调用重绘View
     * <p>
     * 当设置 GLSurfaceView 的渲染模式为 {@link GLSurfaceView#RENDERMODE_CONTINUOUSLY} 或不设置时，
     * 系统就会主动回调onDrawFrame()方法
     * <p>
     * 如果设置为 {@link GLSurfaceView#RENDERMODE_WHEN_DIRTY} ，手动调用requestRender()，才会渲染
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        Log.i(TAG, "onDrawFrame");

        //激活着色器程序
        GLES30.glUseProgram(program);

        //启用顶点属性，顶点属性默认是禁用的
        GLES30.glEnableVertexAttribArray(0);

        //将顶点数据设置给Program，
        //参数一：顶点属性位置
        //参数二：为每个顶点的个数
        //参数三：为数据类型
        //参数四：为是否否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
        //参数五：步长，连续的顶点属性组之间的间隔（当数值是紧密排列时，可以设置0来让OpenGL决定具体步长是多少）
        GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT,
                false, 0, createBuffer());

        //参数一为绘制方式；参数二为从数组缓存中哪一位开始画；参数三为绘制顶点数量
        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, 3);
    }

    //顶点数据
    private FloatBuffer createBuffer(){
        //三个顶点数据，每个顶点数据包含x、y、z三个值
        float[] vertexArray = {
                0.0f, 0.5f, 0.0f,   // top
                -0.5f, -0.5f, 0.0f, // bottom left
                0.5f, -0.5f, 0.0f}; // bottom right

        //每个值占用4个字节（32位）
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(vertexArray.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer buffer = byteBuffer.asFloatBuffer();
        buffer.put(vertexArray);
        buffer.position(0);
        return buffer;
    }
}
