package com.whf.opengldemo.utils;

import android.opengl.GLES30;
import android.util.Log;

public class ShaderUtils {

    private static final String TAG = "WHF_"+ShaderUtils.class.getSimpleName();

    /**
     * 编译着色器
     * @param shaderType {@link GLES30#GL_VERTEX_SHADER} {@link GLES30#GL_FRAGMENT_SHADER}
     * @param shaderSource glsl源码
     * @return
     */
    private static int compileShader(int shaderType,String shaderSource){
        //创建空Shader，返回值为Shader句柄
        int shader = GLES30.glCreateShader(shaderType);
        if (shader == 0){
            return 0;
        }

        //加载 Shader 源码
        GLES30.glShaderSource(shader,shaderSource);

        //编译 Shader
        GLES30.glCompileShader(shader);

        //获取 Shader 编译结果
        int[] compileStatus = new int[1];
        GLES30.glGetShaderiv(shader,GLES30.GL_COMPILE_STATUS,compileStatus,0);

        //编译失败
        if (compileStatus[0] == 0){
            Log.e(TAG,"compileShader error = "+GLES30.glGetShaderInfoLog(shader));
            GLES30.glDeleteShader(shader);
            shader = 0;
        }

        return shader;
    }

    /**
     * 链接 着色器 为 着色器程序
     * @param vertexSource 顶点着色器源码
     * @param fragmentSource 片元着色器源码
     */
    public static int createAndLinkProgram(String vertexSource,String fragmentSource){
        //创建着色器程序
        int program = GLES30.glCreateProgram();
        if (program == 0){
            return program;
        }

        //编译顶点着色器
        int vertexShader = compileShader(GLES30.GL_VERTEX_SHADER,vertexSource);
        //编译片段着色器
        int fragmentShader = compileShader(GLES30.GL_FRAGMENT_SHADER,fragmentSource);

        //将顶点着色器附加到着色器程序
        GLES30.glAttachShader(program,vertexShader);
        //将片段着色器附加到着色器程序
        GLES30.glAttachShader(program,fragmentShader);

        //链接所有着色器为一个着色器程序
        GLES30.glLinkProgram(program);

        //获取链接状态
        int[] linkStatus = new int[1];
        GLES30.glGetProgramiv(program,GLES30.GL_LINK_STATUS,linkStatus,0);

        if (linkStatus[0] == 0){
            Log.e(TAG,"createAndLinkProgram error = "+GLES30.glGetProgramInfoLog(program));
            GLES30.glDeleteProgram(program);
            program = 0;
        }

        return program;
    }
}
