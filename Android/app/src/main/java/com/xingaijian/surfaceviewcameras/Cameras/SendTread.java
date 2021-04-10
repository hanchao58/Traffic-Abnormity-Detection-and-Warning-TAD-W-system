package com.xingaijian.surfaceviewcameras.Cameras;

import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;

import static com.xingaijian.surfaceviewcameras.Cameras.SurfaceViewCallback.packetList;
import static com.xingaijian.surfaceviewcameras.Cameras.SurfaceViewCallback.packetSender;
import static com.xingaijian.surfaceviewcameras.Cameras.SurfaceViewCallback.packet;

public class SendTread extends Thread {
    /*public static DatagramSocket packetSender;

    static {
        try {
            packetSender = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }*/


    @Override
    public void run() {
        super.run();

        Log.i("tread", "run:4 ");
        DatagramPacket packet1;
        while (packetSender != null) {
            /*Log.i("tread", "run:1 ");*/
            /*DatagramPacket packet1;*/
            synchronized (packetList) {
                if (packetList.isEmpty()) {
                    Log.i("tread", "空 ");
                    try {
                        Thread.sleep(5);
                        continue;
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                //取出队头

                packet1 = packetList.getFirst();
                packetList.removeFirst();
                Log.i("packet", "onCreate: do");
            }
            try {
                packetSender.send(packet1);
                Log.i("packet", "send");
            } catch (IOException e) {
                e.printStackTrace();

            }
         /*else{
                    Log.i("packet", "no");
                }*/
        }
    }
}

/*private class ImgSendThread extends Thread {
    @Override
    public void run() {
        super.run();
        while (packetSenderSocket != null) {
            DatagramPacket packet;
            synchronized (packetList) {
                //没有待发送的包
                if (packetList.isEmpty()) {
                    try {
                        Thread.sleep(10);
                        continue;
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                //取出队头
                packet = packetList.getFirst();
                packetList.removeFirst();
            }
            try {
                packetSenderSocket.send(packet);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}*/

