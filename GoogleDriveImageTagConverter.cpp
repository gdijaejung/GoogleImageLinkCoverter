// GoogleDriveImageTagConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
using namespace std;

#define ERROR_PROC() \
{\
CloseClipboard();\
::MessageBoxA(NULL, "컨버팅 실패", "Error", MB_OK);\
return 0;\
}


int _tmain(int argc, _TCHAR* argv[])
{
	// 클립보드 정보 읽기
	::OpenClipboard(NULL);
	HANDLE hClipboard = GetClipboardData(CF_TEXT);
	if (!hClipboard)
		ERROR_PROC();

	string str = (char*)GlobalLock(hClipboard);
	CloseClipboard();

	const int idx0 = str.find('=');
	if (string::npos == idx0)
		ERROR_PROC();
	
	const int idx1 = str.find('&', idx0);
	if (string::npos == idx1)
		ERROR_PROC();

	// 이미 컨버팅 되었다면, 컨버팅하지 않는다.
	if (string::npos != str.find("export=view"))
	{
		::MessageBoxA(NULL, "이미 컨버팅 되어서 클립보드에 저장되어 있습니다.", "Msg", MB_OK);
		return 0;
	}

	string imageLink = str.substr(idx0+1, idx1 - idx0 - 1);
	string cvtURL = "http://drive.google.com/uc?export=view&id=" + imageLink;


	// 클립보드에 저장
	::OpenClipboard(NULL);
	EmptyClipboard();

	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 256);
	char *mem = (char*)GlobalLock(hglbCopy);
	strcpy_s(mem, 256, cvtURL.c_str());
	GlobalUnlock(hglbCopy);
	::SetClipboardData(CF_TEXT, mem);
	CloseClipboard();


	// 변환된 URL을 메세지창에 보여준다.
	string msg = "클립보드에 새 링크가 복사 되었습니다.\nsrc link = " + str + "\n\n" + "convert link = " + cvtURL;
	::MessageBoxA(NULL, msg.c_str(), "Msg", MB_OK);

	return 0;
}

