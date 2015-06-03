// GoogleDriveImageTagConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
using namespace std;

#define ERROR_PROC() \
{\
CloseClipboard();\
::MessageBoxA(NULL, "������ ����", "Error", MB_OK);\
return 0;\
}


int _tmain(int argc, _TCHAR* argv[])
{
	// Ŭ������ ���� �б�
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

	// �̹� ������ �Ǿ��ٸ�, ���������� �ʴ´�.
	if (string::npos != str.find("export=view"))
	{
		::MessageBoxA(NULL, "�̹� ������ �Ǿ Ŭ�����忡 ����Ǿ� �ֽ��ϴ�.", "Msg", MB_OK);
		return 0;
	}

	string imageLink = str.substr(idx0+1, idx1 - idx0 - 1);
	string cvtURL = "http://drive.google.com/uc?export=view&id=" + imageLink;


	// Ŭ�����忡 ����
	::OpenClipboard(NULL);
	EmptyClipboard();

	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 256);
	char *mem = (char*)GlobalLock(hglbCopy);
	strcpy_s(mem, 256, cvtURL.c_str());
	GlobalUnlock(hglbCopy);
	::SetClipboardData(CF_TEXT, mem);
	CloseClipboard();


	// ��ȯ�� URL�� �޼���â�� �����ش�.
	string msg = "Ŭ�����忡 �� ��ũ�� ���� �Ǿ����ϴ�.\nsrc link = " + str + "\n\n" + "convert link = " + cvtURL;
	::MessageBoxA(NULL, msg.c_str(), "Msg", MB_OK);

	return 0;
}

