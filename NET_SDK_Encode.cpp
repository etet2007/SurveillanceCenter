
#include "NET_SDK_Encode.h"
#include<QDebug>
#pragma warning(disable: 4996)


CRITICAL_SECTION g_cs_frameList;
//extern LONG lUserID;
LONG lRealPlayHandle;
void CALLBACK g_RealDataCallBack_V30(LONG playHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dwUser);

bool YV12ToRGB24_Native(unsigned char* pYUV,unsigned char* pRGB24,int width,int height)
{
    if (width < 1 || height < 1 || pYUV == NULL || pRGB24 == NULL)
        return false;
    const long len = width * height;
    unsigned char* yData = pYUV;
    unsigned char* vData = &yData[len];
    unsigned char* uData = &vData[len >> 2];

    int bgr[3];
    int yIdx,uIdx,vIdx,idx;
    for (int i = 0;i < height;i++){
        for (int j = 0;j < width;j++){
            yIdx = i * width + j;
            vIdx = (i/2) * (width/2) + (j/2);
            uIdx = vIdx;

            bgr[2] = (int)(yData[yIdx] + 1.732446 * (uData[vIdx] - 128));                                    // b分量
            bgr[1] = (int)(yData[yIdx] - 0.698001 * (uData[uIdx] - 128) - 0.703125 * (vData[vIdx] - 128));    // g分量
            bgr[0] = (int)(yData[yIdx] + 1.370705 * (vData[uIdx] - 128));                                    // r分量

            for (int k = 0;k < 3;k++){
                idx = (i * width + j) * 3 + k;
                if(bgr[k] >= 0 && bgr[k] <= 255)
                    pRGB24[idx] = bgr[k];
                else
                    pRGB24[idx] = (bgr[k] < 0)?0:255;
            }
        }
    }
    return true;
}

LONG lPort; //全局的播放库port号
camInfo cam, *pCam = NULL; // 全局的存放摄像头参数
bool isPlaying = false; // 全局的视频播放状态标志位

//--------------------------------------------
int iPicNum = 0;//Set channel NO.
LONG nPort = -1;
HWND hWnd = NULL;


//解码回调 视频为YUV数据(YV12)，音频为PCM数据
void CALLBACK DecCBFun(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{

    long lFrameType = pFrameInfo->nType;    
    if(lFrameType ==T_YV12)
    {
        QImage image(pFrameInfo->nWidth,pFrameInfo->nHeight,QImage::Format_RGB888);
        YV12ToRGB24_Native((unsigned char*)pBuf,image.bits(),pFrameInfo->nWidth,pFrameInfo->nHeight);

//        YV12_to_RGB24((unsigned char*)pBuf,image.bits(),pFrameInfo->nWidth,pFrameInfo->nHeight);
        image.save("rgb24.jpg");
//    PlayM4_Stop(nPort);

    }
}


///实时流回调函数
void CALLBACK fRealDataCallBack(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,void *pUser)
{

    DWORD dRet;
    switch (dwDataType)
    {
    case NET_DVR_SYSHEAD:    //系统头
        if (!PlayM4_GetPort(&nPort)) //获取播放库未使用的通道号
        {
            break;
        }
        if(dwBufSize > 0)
        {
            if (!PlayM4_OpenStream(nPort,pBuffer,dwBufSize, 1024 * 1024))//打开流接口

            {
                dRet=PlayM4_GetLastError(nPort);
                break;
            }
            //设置解码回调函数 只解码不显示
            if (!PlayM4_SetDecCallBack(nPort,DecCBFun))
            {
                dRet=PlayM4_GetLastError(nPort);
                break;
            }

            //设置解码回调函数 解码且显示
            //if (!PlayM4_SetDecCallBackEx(nPort,DecCBFun,NULL,NULL))
            //{
            //  dRet=PlayM4_GetLastError(nPort);
            //  break;
            //}

            //打开视频解码
            if (!PlayM4_Play(nPort,hWnd))//播放开始
            {
                dRet=PlayM4_GetLastError(nPort);
                break;
            }

            //打开音频解码, 需要码流是复合流
//          if (!PlayM4_PlaySound(nPort))
//          {
//              dRet=PlayM4_GetLastError(nPort);
//              break;
//          }       
        }
        break;

    case NET_DVR_STREAMDATA:   //码流数据
        if (dwBufSize > 0 && nPort != -1)
        //{
            //BOOL inData=PlayM4_InputData(nPort,pBuffer,dwBufSize);
            //while (!inData)
            //{
            //    Sleep(10);
            //    inData=PlayM4_InputData(nPort,pBuffer,dwBufSize);
            //    OutputDebugString("PlayM4_InputData failed \n");   
            //}
        //}
			if (!PlayM4_InputData(nPort,pBuffer,dwBufSize))
				break;  
    }       
}
//异常回调
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    switch(dwType) 
    {
    case EXCEPTION_RECONNECT:    //预览时重连
//        printf("----------reconnect--------%d\n", time(NULL));
        break;
    default:
        break;
    }
}

//readCamera
int __stdcall readCamera(QString cameraIp)
{
    //---------------------------------------
    // 初始化
//    camInfo* cam = (camInfo*)param;
    NET_DVR_Init();
    //设置连接时间，重连时间，超时时间
    NET_DVR_SetConnectTime(200, 1);
    NET_DVR_SetReconnect(1000, true);
    NET_DVR_SetRecvTimeOut(3000);

//    std::cout << cam->ipAddress << std::endl;
//    std::cout << atoi(cam->port) << std::endl;
//    std::cout << cam->usrName << std::endl;
//    std::cout << cam->password << std::endl;
    //---------------------------------------

    //设置异常消息回调函数
    // NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

    //---------------------------------------
    // 注册设备
    LONG lUserID;

    //登录参数，包括设备地址、登录用户、密码等
//    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
//    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
//    strcpy(struLoginInfo.sDeviceAddress, "192.0.0.64"); //设备IP地址
//    struLoginInfo.wPort = 8000; //设备服务端口
//    strcpy(struLoginInfo.sUserName, "admin"); //设备登录用户名
//    strcpy(struLoginInfo.sPassword, "admin@12345"); //设备登录密码

    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
//    lUserID = NET_DVR_Login_V30(cam->ipAddress, atoi(cam->port), cam->usrName, cam->password, &struDeviceInfo); // "192.168.153.201"
    QByteArray ba = cameraIp.toLatin1();
    char *cameraIpCharPtr = ba.data();
    lUserID = NET_DVR_Login_V30(cameraIpCharPtr, 8000,"admin", "admin@12345", &struDeviceInfo); // "192.168.153.201"

    if (lUserID < 0)
    {
        NET_DVR_Cleanup();
        isPlaying = false; // 播放失败
		return -1;
    }
    
	NET_DVR_PREVIEWINFO struPlayInfo = {0};
    struPlayInfo.hPlayWnd     = NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel     = 1;       //预览通道号
	struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP

    bool isSet=NET_DVR_SetCapturePictureMode(JPEG_MODE);

    //定义JPEG图像质量
    LPNET_DVR_JPEGPARA JpegPara = new NET_DVR_JPEGPARA;
    JpegPara->wPicQuality = 0;
    JpegPara->wPicSize = 9;
//  可用
    QByteArray byteArrayfileName = cameraIp.append(".jpg").toLatin1();
    char *fileNameCharPtr = byteArrayfileName.data();
    bool capture = NET_DVR_CaptureJPEGPicture(lUserID,1,JpegPara,fileNameCharPtr);

    qDebug()<<"capture? "<<capture;

    //暂时还不可用
//    QImage image(1920,1080,QImage::Format_RGB888);
//    DWORD len = 1920*1080;
//    LPDWORD Ret = 0;
//    bool capture = NET_DVR_CaptureJPEGPicture_NEW(lUserID,1,JpegPara,(char*)image.bits(),len,Ret);
//    image.save("test2.jpg");


//    lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, fRealDataCallBack, NULL);//实时预览

//    if (lRealPlayHandle < 0)
//    {
//        isPlaying = false; // 播放失败
//        printf("NET_DVR_RealPlay_V30 failed! Error number: %d\n",NET_DVR_GetLastError());
//        return 0;
//    }

//    isPlaying = true;

//    Sleep(-1);

    //---------------------------------------
    //关闭预览
//    if(!NET_DVR_StopRealPlay(lRealPlayHandle))
//    {
//        printf("NET_DVR_StopRealPlay error! Error number: %d\n",NET_DVR_GetLastError());
//        return 0;
//    }

    //注销用户
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();

    if(!capture){
       return -1;
    }
    return 0;
}

