
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

static int Table_fv1[256] = { -180, -179, -177, -176, -174, -173, -172,
                              -170, -169, -167, -166, -165, -163, -162,
                              -160, -159, -158, -156, -155, -153, -152,
                              -151, -149, -148, -146, -145, -144, -142,
                              -141, -139, -138, -137, -135, -134, -132,
                              -131, -130, -128, -127, -125, -124, -123,
                              -121, -120, -118, -117, -115, -114, -113,
                              -111, -110, -108, -107, -106, -104, -103,
                              -101, -100, -99, -97, -96, -94, -93, -92,
                              -90, -89, -87, -86, -85, -83, -82, -80,
                              -79, -78, -76, -75, -73, -72, -71, -69,
                              -68, -66, -65, -64,-62, -61, -59, -58,
                              -57, -55, -54, -52, -51, -50, -48, -47,
                              -45, -44, -43, -41, -40, -38, -37, -36,
                              -34, -33, -31, -30, -29, -27, -26, -24,
                              -23, -22, -20, -19, -17, -16, -15, -13,
                              -12, -10, -9, -8, -6, -5, -3, -2, 0, 1,
                              2, 4, 5, 7, 8, 9, 11, 12, 14, 15, 16, 18,
                              19, 21, 22, 23, 25, 26, 28, 29, 30, 32, 33,
                              35, 36, 37, 39, 40, 42, 43, 44, 46, 47, 49,
                              50, 51, 53, 54, 56, 57, 58, 60, 61, 63, 64,
                              65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 79,
                              81, 82, 84, 85, 86, 88, 89, 91, 92, 93, 95,
                              96, 98, 99, 100, 102, 103, 105, 106, 107,
                              109, 110, 112, 113, 114, 116, 117, 119, 120,
                              122, 123, 124, 126, 127, 129, 130, 131, 133,
                              134, 136, 137, 138, 140, 141, 143, 144, 145,
                              147, 148, 150, 151, 152, 154, 155, 157, 158,
                              159, 161, 162, 164, 165, 166, 168, 169, 171,
                              172, 173, 175, 176, 178 };
static int Table_fv2[256] = { -92, -91, -91, -90, -89, -88, -88, -87, -86,
                              -86, -85, -84, -83, -83, -82, -81, -81, -80,
                              -79, -78, -78, -77, -76, -76, -75, -74, -73,
                              -73, -72, -71, -71, -70, -69, -68, -68, -67,
                              -66, -66, -65, -64, -63, -63, -62, -61, -61,
                              -60, -59, -58, -58, -57, -56, -56, -55, -54,
                              -53, -53, -52, -51, -51, -50, -49, -48, -48,
                              -47, -46, -46, -45, -44, -43, -43, -42, -41,
                              -41, -40, -39, -38, -38, -37, -36, -36, -35,
                              -34, -33, -33, -32, -31, -31, -30, -29, -28,
                              -28, -27, -26, -26, -25, -24, -23, -23, -22,
                              -21, -21, -20, -19, -18, -18, -17, -16, -16,
                              -15, -14, -13, -13, -12, -11, -11, -10, -9,
                              -8, -8, -7, -6, -6, -5, -4, -3, -3, -2, -1,
                              0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9, 10,
                              10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 17,
                              18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25,
                              26, 27, 27, 28, 29, 30, 30, 31, 32, 32, 33,
                              34, 35, 35, 36, 37, 37, 38, 39, 40, 40, 41,
                              42, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49,
                              50, 50, 51, 52, 52, 53, 54, 55, 55, 56, 57,
                              57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 65,
                              65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 72,
                              73, 74, 75, 75, 76, 77, 77, 78, 79, 80, 80,
                              81, 82, 82, 83, 84, 85, 85, 86, 87, 87, 88,
                              89, 90, 90 };
static int Table_fu1[256] = { -44, -44, -44, -43, -43, -43, -42, -42, -42,
                              -41, -41, -41, -40, -40, -40, -39, -39, -39,
                              -38, -38, -38, -37, -37, -37, -36, -36, -36,
                              -35, -35, -35, -34, -34, -33, -33, -33, -32,
                              -32, -32, -31, -31, -31, -30, -30, -30, -29,
                              -29, -29, -28, -28, -28, -27, -27, -27, -26,
                              -26, -26, -25, -25, -25, -24, -24, -24, -23,
                              -23, -22, -22, -22, -21, -21, -21, -20, -20,
                              -20, -19, -19, -19, -18, -18, -18, -17, -17,
                              -17, -16, -16, -16, -15, -15, -15, -14, -14,
                              -14, -13, -13, -13, -12, -12, -11, -11, -11,
                              -10, -10, -10, -9, -9, -9, -8, -8, -8, -7,
                              -7, -7, -6, -6, -6, -5, -5, -5, -4, -4, -4,
                              -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 0, 1,
                              1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6,
                              6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11,
                              11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14,
                              15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18,
                              18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22,
                              22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26,
                              26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29,
                              30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33,
                              34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37,
                              37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41,
                              41, 41, 42, 42, 42, 43, 43 };
static int Table_fu2[256] = { -227, -226, -224, -222, -220, -219, -217,
                              -215, -213, -212, -210, -208, -206, -204,
                              -203, -201, -199, -197, -196, -194, -192,
                              -190, -188, -187, -185, -183, -181, -180,
                              -178, -176, -174, -173, -171, -169, -167,
                              -165, -164, -162, -160, -158, -157, -155,
                              -153, -151, -149, -148, -146, -144, -142,
                              -141, -139, -137, -135, -134, -132, -130,
                              -128, -126, -125, -123, -121, -119, -118,
                              -116, -114, -112, -110, -109, -107, -105,
                              -103, -102, -100, -98, -96, -94, -93, -91,
                              -89, -87, -86, -84, -82, -80, -79, -77, -75,
                              -73, -71, -70, -68, -66, -64, -63, -61, -59,
                              -57, -55, -54, -52, -50, -48, -47, -45, -43,
                              -41, -40, -38, -36, -34, -32, -31, -29, -27,
                              -25, -24, -22, -20, -18, -16, -15, -13, -11,
                              -9, -8, -6, -4, -2, 0, 1, 3, 5, 7, 8, 10, 12,
                              14, 15, 17, 19, 21, 23, 24, 26, 28, 30, 31,
                              33, 35, 37, 39, 40, 42, 44, 46, 47, 49, 51,
                              53, 54, 56, 58, 60, 62, 63, 65, 67, 69, 70,
                              72, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90,
                              92, 93, 95, 97, 99, 101, 102, 104, 106, 108,
                              109, 111, 113, 115, 117, 118, 120, 122, 124,
                              125, 127, 129, 131, 133, 134, 136, 138, 140,
                              141, 143, 145, 147, 148, 150, 152, 154, 156,
                              157, 159, 161, 163, 164, 166, 168, 170, 172,
                              173, 175, 177, 179, 180, 182, 184, 186, 187,
                              189, 191, 193, 195, 196, 198, 200, 202, 203,
                              205, 207, 209, 211, 212, 214, 216, 218, 219,
                              221, 223, 225 };

bool YV12ToBGR24_Table(unsigned char* pYUV,unsigned char* pBGR24,int width,int height)
{
    if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
        return false;
    const long len = width * height;
    unsigned char* yData = pYUV;
    unsigned char* vData = &yData[len];
    unsigned char* uData = &vData[len >> 2];
    int bgr[3];
    int yIdx,uIdx,vIdx,idx;
    int rdif,invgdif,bdif;
    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            yIdx = i * width + j;
            vIdx = (i/2) * (width/2) + (j/2);
            uIdx = vIdx;
            rdif = Table_fv1[vData[vIdx]];
            invgdif = Table_fu1[uData[uIdx]] + Table_fv2[vData[vIdx]];
            bdif = Table_fu2[uData[uIdx]];
            bgr[0] = yData[yIdx] + bdif; bgr[1] = yData[yIdx] - invgdif;
            bgr[2] = yData[yIdx] + rdif;
            for (int k = 0;k < 3;k++)
            {
                idx = (i * width + j) * 3 + k;
                if(bgr[k] >= 0 && bgr[k] <= 255)
                    pBGR24[idx] = bgr[k];
                else
                    pBGR24[idx] = (bgr[k] < 0)?0:255;
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

//readCamera 返回-1失败 0成功
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

    char username[]="admin";
    char password[]="admin@12345";
    lUserID = NET_DVR_Login_V30(cameraIpCharPtr, 8000,username, password, &struDeviceInfo); // "192.168.153.201"

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
    return 1;
}

