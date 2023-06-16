
// �ʿ��� ��� ���ϵ�
#include <stdio.h>          // ǥ�� �Է�/��� ���
#include <stdlib.h>         // ǥ�� ���̺귯�� ���
#include <string.h>         // ���ڿ� ó�� ���̺귯��
#include <WinSock2.h>       // ������ ���� 2 ���
#include <WS2tcpip.h>       // WinSock2�� �߰� ����� �����ϴ� ���
#pragma comment(lib, "ws2_32.lib")  // WinSock2�� ���� ��Ŀ ���ù�

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

	// UDP�̱� ������ Ÿ���� SOCK_DGRAM���� ����
	hServSock = socket(PF_INET, SOCK_DGRAM, 0);

	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	// ���� UDP�̱� ������ bind���� listen�� ���� �κ� x

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