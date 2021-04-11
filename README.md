# Traffic Abnormity Detection and Warning (TAD&W) system
基于Android、C++与python的实时视频传输与预警

假设在一个道路场景中，前车有Android系统设备，后车有Windows系统设备。我们做到了前车能够实时把自己的画面分享给后车，这是通过无线传输来实现的。而后车要做的事情就是用接收的画面判断前方有没有发生事故，以便及时预警。

Android端从摄像头获取前方画面，因为数据量过大，所以我们对每一帧图片进行压缩、编码，然后通过OBU A广播出去，OBU B接收数据。除此之外也可以有多个OBU同时接收OBU A的数据。这里用到的是UDP传输。OBU的通信是靠DSRC专用短程通信技术实现的。在OBU B实时把数据传给Windows端时，Windows端会对接收的数据解码，并调用检测模型进行检测。当出现事故时，能及时提醒驾驶员。整个系统我们用到了Android、C++以及python编程。

