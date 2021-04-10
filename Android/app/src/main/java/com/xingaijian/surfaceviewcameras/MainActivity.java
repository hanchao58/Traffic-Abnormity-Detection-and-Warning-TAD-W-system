package com.xingaijian.surfaceviewcameras;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

import com.xingaijian.surfaceviewcameras.Cameras.CameraSurfaceHolder;
import com.xingaijian.surfaceviewcameras.Cameras.SendTread;
import com.xingaijian.surfaceviewcameras.Cameras.SurfaceViewCallback;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.LinkedList;

public class MainActivity extends AppCompatActivity {

    private SurfaceView mainSfCamera;

    SendTread sendtread = new SendTread();
    CameraSurfaceHolder mCameraSurfaceHolder = new CameraSurfaceHolder();

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mainSfCamera = (SurfaceView) findViewById(R.id.main_sf_camera);

        // android 6.0以上动态申请权限  或targetSdkVersion设置为23以下

        if (checkCameraHardware(MainActivity.this)) {
            mCameraSurfaceHolder.setCameraSurfaceHolder(this, mainSfCamera);
        }
        sendtread.start();

    }

    private boolean checkCameraHardware(Context context) {
        if (context.getPackageManager().hasSystemFeature(PackageManager.FEATURE_CAMERA)) {
            Toast.makeText(this, "搜索到摄像头硬件", Toast.LENGTH_SHORT).show();
            return true;
        } else {
            Toast.makeText(this, "不具备摄像头硬件", Toast.LENGTH_SHORT).show();
            return false;
        }
    }

}


