package com.whf.opengldemo;

import android.app.Activity;
import android.app.Person;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.whf.opengldemo.one.OneActivity;
import com.whf.opengldemo.three.ThreeActivity;
import com.whf.opengldemo.two.TwoActivity;


public class MainActivity extends Activity implements View.OnClickListener {

    public static final String COMMON_TAG = "WHF_";
    private static final String TAG = COMMON_TAG + MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.btn_gl_java).setOnClickListener(this);
        findViewById(R.id.btn_gl_native).setOnClickListener(this);
        findViewById(R.id.btn_egl_java).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_gl_java:
                startActivity(new Intent(this, OneActivity.class));
                break;
            case R.id.btn_gl_native:
                startActivity(new Intent(this, TwoActivity.class));
                break;
            case R.id.btn_egl_java:
                startActivity(new Intent(this, ThreeActivity.class));
                break;
        }
    }
}