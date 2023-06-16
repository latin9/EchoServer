
// 필요한 헤더 파일들
#include <stdio.h>          // 표준 입력/출력 헤더
#include <stdlib.h>         // 표준 라이브러리 헤더
#include <string.h>         // 문자열 처리 라이브러리
#include <WinSock2.h>       // 윈도우 소켓 2 헤더
#include <WS2tcpip.h>       // WinSock2에 추가 기능을 제공하는 헤더
#pragma comment(lib, "ws2_32.lib")  // WinSock2를 위한 링커 지시문

#define BUFSIZE 30
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA		wsaData;
	SOCKET		hServSock;
	char message[BUFSIZE];
	int strLen;
	SOCKADDR_IN	servAddr;
	SOCKADDR_IN	clntAddr;
	int clntAddrSize;

	if (argc != 2)
	{
		printf_s("USage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	// UDP이기 때문에 타입을 SOCK_DGRAM으로 지정
	hServSock = socket(PF_INET, SOCK_DGRAM, 0);

	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	// 또한 UDP이기 때문에 bind이후 listen과 같은 부분 x

	while (1)
	{
		clntAddrSize = sizeof(clntAddr);

		strLen = recvfrom(hServSock, message, BUFSIZE, 0, (SOCKADDR*)&clntAddr, &clntAddrSize);

		sendto(hServSock, message, strLen, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
	}

	closesocket(hServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}