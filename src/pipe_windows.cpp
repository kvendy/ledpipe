#include "pipe_windows.h"
#include <iostream>

#define BUFSIZE 512

Pipe::Pipe(const std::string& name) :
	pipeName(name),
	hPipe(INVALID_HANDLE_VALUE)
{
	BOOL   fConnected = FALSE;
	DWORD  dwThreadId = 0;
	HANDLE hThread = NULL;
	std::wstring stemp = std::wstring(pipeName.begin(), pipeName.end());
	LPCWSTR lpszPipename = stemp.c_str();
	hPipe = ::CreateNamedPipeW(
		lpszPipename,             // pipe name 
		PIPE_ACCESS_DUPLEX,       // read/write access 
		PIPE_TYPE_MESSAGE |       // message type pipe 
		PIPE_READMODE_MESSAGE |   // message-read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // max. instances  
		BUFSIZE,                  // output buffer size 
		BUFSIZE,                  // input buffer size 
		0,                        // client time-out 
		NULL);                    // default security attribute 

	if (hPipe == INVALID_HANDLE_VALUE) 
	{
		std::cout << "Cannot create pipe" << std::endl;
	}
	else
		std::cout << "Pipe created" << std::endl;
}

Pipe::~Pipe()
{
	::CloseHandle(hPipe);
}

std::string Pipe::read()
{
	char data[BUFSIZE];
	DWORD numRead;

	::ConnectNamedPipe(hPipe, NULL);
	::ReadFile(hPipe, data, BUFSIZE, &numRead, NULL);
	::DisconnectNamedPipe(hPipe);

	std::string content(data, 0, numRead);

	return content;
}

void Pipe::write(const std::string& data)
{
	::ConnectNamedPipe(hPipe, NULL);
	std::wstring stemp = std::wstring(data.begin(), data.end());
	LPCWSTR wData = stemp.c_str();
	DWORD numBytesWritten = 0,
	result = ::WriteFile(
	         hPipe,                           // handle to our outbound pipe
	         wData,                           // data to send
	         wcslen(wData) * sizeof(wchar_t), // length of data to send (bytes)
	         &numBytesWritten,                // will store actual amount of data sent
	         NULL                             // not using overlapped IO
		);
	DisconnectNamedPipe(hPipe);
}
