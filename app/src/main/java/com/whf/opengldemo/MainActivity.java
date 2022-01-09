package com.whf.opengldemo;

import static com.whf.opengldemo.Constants.COMMON_TAG;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.whf.opengldemo.part1.Part1Activity;
import com.whf.opengldemo.part3.Part3Activity;
import com.whf.opengldemo.part2.Part2Activity;


public class MainActivity extends Activity implements View.OnClickListener {

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
                startActivity(new Intent(this, Part1Activity.class));
                break;
            case R.id.btn_gl_native:
                startActivity(new Intent(this, Part2Activity.class));
                break;
            case R.id.btn_egl_java:
                startActivity(new Intent(this, Part3Activity.class));
                break;
        }
    }
}