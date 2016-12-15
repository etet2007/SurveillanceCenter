#ifndef NET_SDK_ENCODE_H
#define NET_SDK_ENCODE_H
#pragma once

#include <Windows.h>
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include <QMessageBox>

//using namespace cv;

typedef unsigned char uchar;

void yv12toYUV(char *outYuv, char *inYv12, int width, int height,int widthStep);
void CALLBACK DecCBFun(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);
// 实时流回调
void CALLBACK fRealDataCallBack(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,void *pUser);
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
//unsigned int __stdcall readCamera(void *param);
int __stdcall readCamera(QString cameraIp);
struct camInfo{
    char ipAddress[16];
    char port[6];
    char usrName[20];
    char password[20];
};

#endif // NET_SDK_ENCODE_H
