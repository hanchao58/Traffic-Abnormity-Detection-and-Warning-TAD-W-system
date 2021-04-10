package com.xingaijian.surfaceviewcameras.Cameras;

import android.app.Activity;
import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.xingaijian.surfaceviewcameras.MainActivity;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.lang.reflect.Array;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.concurrent.TimeUnit;

/**
 * 相机界面SurfaceView的回调类
 */
public final class SurfaceViewCallback implements SurfaceHolder.Callback, Camera.PreviewCallback {
    public static DatagramSocket packetSender;

    static {
        try {
            packetSender = new DatagramSocket(4040);
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }


    public final static LinkedList<DatagramPacket> packetList = new LinkedList<>();
    private SurfaceView mainSfCamera;
    private final int UDP_PORT = 4040;                  //UDP通讯的端口号
    private final String SERVER_IP = "169.254.157.2" + "";   //服务器端的IP地址

    private Camera.Size previewSize;
    private long lastSendTime;                  //上一次发送图像帧的时间
    InetAddress serverAddress;          //服务端地址
    static DatagramPacket packet;
    Context context;
    static final String TAG = "SYRFACECamera";
    FrontCamera mFrontCamera = new FrontCamera();
    boolean previewing = mFrontCamera.getPreviewing();
    Camera mCamera;
    FaceTask mFaceTask;


    public void setContext(Context context) {
        this.context = context;
    }

    public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
        if (previewing) {
            mCamera.stopPreview();
            Log.i(TAG, "停止预览");
        }

        try {
            /*Camera.Parameters parameters =null;
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);*/
            mCamera.setPreviewDisplay(arg0);
            mCamera.startPreview();
            mCamera.setPreviewCallback(this);

            Log.i(TAG, "开始预览");
            //调用旋转屏幕时自适应
            //setCameraDisplayOrientation(MainActivity.this, mCurrentCamIndex, mCamera);
        } catch (Exception e) {
        }
    }

    public void surfaceCreated(SurfaceHolder holder) {
        //初始化前置摄像头
        mFrontCamera.setCamera(mCamera);
        mCamera = mFrontCamera.initCamera();
        mCamera.setPreviewCallback(this);
        mCamera.cancelAutoFocus();

        //适配竖排固定角度
        FrontCamera.setCameraDisplayOrientation((Activity) context, mFrontCamera.getCurrentCamIndex(), mCamera);
        Log.i(TAG, "surfaceCreated");
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        mFrontCamera.StopCamera(mCamera);
        Log.i(TAG, "surfaceDestroyed");
    }

    /**
     * 相机实时数据的回调
     *
     * @param data   相机获取的数据，格式是YUV
     * @param camera 相应相机的对象
     */
    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        Camera.Size size = mCamera.getParameters().getPreviewSize(); //获取预览大小
        long curTime = System.currentTimeMillis();
        int i;
        if (curTime - lastSendTime >=150){

            lastSendTime = curTime;
            //NV21格式转JPEG格式
            YuvImage image = new YuvImage(data, ImageFormat.NV21, size.width, size.height, null);
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            image.compressToJpeg(new Rect(0, 0, size.width, size.height), 20, bos);
            int packMaxSize = 14000;    //防止超过UDP包的最大大小
            byte[] imgBytes = bos.toByteArray();
            String Str1 = new String("end "+imgBytes.length);
            /*String Str2 = new String(imgBytes.length+"");
            byte[] x=Str2.getBytes();*/
            byte[] c = Str1.getBytes();
            ByteBuffer byteBuffer = ByteBuffer.allocate(500000);
            Log.i("tag", imgBytes.length + "");
            if (imgBytes.length > packMaxSize) {
                int a=(imgBytes.length/14000)+1;
                int b=imgBytes.length / a;
                byte[] byte1 = new byte[b];
                /*byteBuffer.clear();*/
                byteBuffer.put(imgBytes, 0, imgBytes.length);
                System.out.println(byteBuffer);
                byteBuffer.flip();
                for (i = 0; i < a; i++) {
                    byteBuffer.get(byte1, 0, b);
                    Log.i("tag", byte1.length + "");
                    try {
                        packet = new DatagramPacket(byte1, Math.min(byte1.length, packMaxSize),
                                InetAddress.getByName(SERVER_IP), UDP_PORT);
                    } catch (UnknownHostException e) {
                        e.printStackTrace();
                    }

                    Log.i("run", " 打包");
                    packetList.addLast(packet);
                    byte1 = new byte[b];
                    byteBuffer.compact();
                    try {
                        TimeUnit.MICROSECONDS.sleep(2);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    byteBuffer.clear();
                }
                byteBuffer.compact();

            } else {
                try {
                    Log.i("tag", imgBytes.length + "");
                    packet = new DatagramPacket(imgBytes, Math.min(imgBytes.length, packMaxSize),
                            InetAddress.getByName(SERVER_IP), UDP_PORT);
                } catch (UnknownHostException e) {
                    e.printStackTrace();
                }

                Log.i("run", " 打包");
                //添加到队尾
                packetList.addLast(packet);
            }
            /*Log.i("tread", " 7");*/
            try {
                packet = new DatagramPacket(c, Math.min(c.length, packMaxSize),
                        InetAddress.getByName(SERVER_IP), UDP_PORT);
            } catch (UnknownHostException e) {
                e.printStackTrace();
            }
            packetList.addLast(packet);
            /*try {
                packet = new DatagramPacket(x, Math.min(x.length, packMaxSize),
                        InetAddress.getByName(SERVER_IP), UDP_PORT);
            } catch (UnknownHostException e) {
                e.printStackTrace();
            }
            packetList.addLast(packet);*/
            Log.i("run", c.length+"");
            /*try {
                TimeUnit.MICROSECONDS.sleep(5);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }*/
        }
    }
}

