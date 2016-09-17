#pragma once;
#ifdef __DLL__API
#define DLL_API __declspec(dllexport)  //导出函数方式
#else  
#define DLL_API __declspec(dllimport) 
#endif
#include<string>
#include<windows.h>
using namespace std;
struct MPOINT
{
	float x;
	float y;

};

class Myhttpclient
{
public:
	Myhttpclient();
	~Myhttpclient();
public:
	int deleteid(int &id);
	int put(int &id,string &ip,MPOINT &x,MPOINT &xa,MPOINT &xb,MPOINT &xc,MPOINT &xd);
	int post(string &ip,MPOINT &x,MPOINT &xa,MPOINT &xb,MPOINT &xc,MPOINT &xd);
	int get(MPOINT &A,MPOINT &B,MPOINT &C,MPOINT &D,string &response);
	int gets();
	int postimage(char*strimage, char*id, char*ip);
	int getimage(char*location,string&picture);
	void setdebug(bool bdebug);
	static DWORD WINAPI Fun1Proc(LPVOID lpParameter);
	static DWORD WINAPI Fun2Proc(LPVOID lpParameter);
private:
	 bool m_bdebug;
};

