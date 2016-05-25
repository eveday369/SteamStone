#pragma once

#include <iostream>
#include <conio.h>
#include <process.h>
#include <WinSock2.h>
#include <MMSystem.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")

#define IP_LENTH	256
#define PORT_LENTH	16
#define ID_LENTH	64
#define	PS_LENTH	64
#define CMD_LENTH	512
#define BUFFER_SIZE 1024

using namespace std;

typedef enum {OP_RECV, OP_SEND}OPERATION_TYPE;

typedef struct stOverlappedEx
{
	WSAOVERLAPPED	m_wsaOverlapped;
	SOCKET			m_socketClinet;
	WSABUF			m_wsaBuf;
	BYTE			m_szBuf[BUFFER_SIZE];
	OPERATION_TYPE	m_eOperation;
}OVERLAPPEDEX;

typedef struct stSockInfo
{
	SOCKET			m_socket;
	stOverlappedEx	m_stRecvOverlappedEx;
	stOverlappedEx	m_stSendOverlappedEx;
	stSockInfo()
	{
		m_socket = INVALID_SOCKET;
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
	}
}SOCKETINFO;