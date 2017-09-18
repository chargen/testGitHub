// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#endif // _WIN32
//#include "Common/b3Vector3.h"
//#include "Common/b3Matrix3x3.h"
//#include "duktape/duk_config.h"
#include "duktape/duktape.h"
#ifdef _WIN32
using namespace std;
#endif // _WIN32

int m_frameWidth = 1280, m_frameHeight = 720;
void CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}

void RunCmdline(string cmdline)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	cmdline = "cmd.exe /C " + cmdline;
	printf("cmd %s", cmdline.c_str());
	TCHAR *cmd = new TCHAR[strlen(cmdline.c_str())];
	CharToTchar(cmdline.c_str(), cmd);
	
	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		(LPTSTR)cmd,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return;
}

std::tr1::shared_ptr<unsigned char> RevertFrameBuffer(std::tr1::shared_ptr<unsigned char> frame)
{
	int size = m_frameWidth*m_frameHeight * 4;
	auto temp_frame = std::tr1::shared_ptr<unsigned char>(new unsigned char[size]);

	for (int i = 0; i < m_frameHeight; i++)
	{
		auto ptr = frame.get() + i*m_frameWidth * 4 + m_frameWidth * 4;
		auto qptr = temp_frame.get() + i*m_frameWidth * 4;
		for (int j = 0; j < m_frameWidth; j++)
		{
			qptr[0] = ptr[0];
			qptr[1] = ptr[1];
			qptr[2] = ptr[2];
			qptr[3] = ptr[3];
			ptr -= 4;
			qptr += 4;
		}
	}

	return temp_frame;
}

std::tr1::shared_ptr<unsigned char> RevertFrameBufferV(std::tr1::shared_ptr<unsigned char> frame)
{
	int size = m_frameWidth*m_frameHeight * 4;
	auto temp_frame = std::tr1::shared_ptr<unsigned char>(new unsigned char[size]);

	for (int i = 0; i < m_frameWidth; i++)
	{
		auto ptr = frame.get() + i * 4 + (m_frameHeight - 1) * m_frameWidth * 4;
		auto qptr = temp_frame.get() + i * 4;
		for (int j = 0; j < m_frameHeight; j++)
		{
			qptr[0] = ptr[0];
			qptr[1] = ptr[1];
			qptr[2] = ptr[2];
			qptr[3] = ptr[3];
			ptr -= 4 * m_frameWidth;
			qptr += 4 * m_frameWidth;
		}
	}

	return temp_frame;
}

void ScissorFrameBuffer(std::tr1::shared_ptr<unsigned char> frame)
{
	int size = m_frameWidth*m_frameHeight * 4;
	for (int i = 0; i < m_frameHeight; i++)
	{
		auto ptr = frame.get() + i*m_frameWidth * 4 + m_frameWidth * 4;
		auto qptr = frame.get() + i*m_frameWidth * 4;
		for (int j = 0; j < m_frameWidth * 0.31640625f; j++)
		{
			qptr[0] = 0;
			qptr[1] = 0;
			qptr[2] = 0;
			qptr[3] = 0;
			qptr += 4;
			ptr[0] = 0;
			ptr[1] = 0;
			ptr[2] = 0;
			ptr[3] = 0;
			ptr -= 4;
		}
	}
}

void main(int argc, TCHAR *argv[])
{

	system("pause");
}

