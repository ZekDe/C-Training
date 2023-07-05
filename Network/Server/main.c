#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define SERVER_PORTNO			55000
#define BUFFER_SIZE				1024

void ExitSys(LPCSTR lpszMsg, DWORD dwLastErr);

int main(void)
{
	WSADATA wsaData;
	DWORD dwResult;
	SOCKET serverSock/*passive(listening) socket*/, clientSock/*talk with this socket*/;
	struct sockaddr_in sinServer, sinClient;
	int addrLen;
	char buf[BUFFER_SIZE];
	char* str;
	int result;

	if ((dwResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
		ExitSys("WSAStartup", dwResult);

	if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 	/* third parameter IPPROTO_TCP */
		ExitSys("socket", WSAGetLastError());

	sinServer.sin_family = AF_INET;
	sinServer.sin_port = htons(SERVER_PORTNO);
	sinServer.sin_addr.s_addr = htonl(INADDR_ANY); // handle all network info

	if (bind(serverSock, (struct sockaddr*)&sinServer, sizeof(sinServer)) == SOCKET_ERROR)
		ExitSys("bind", WSAGetLastError());

	if (listen(serverSock, 8) == SOCKET_ERROR)
		ExitSys("listen", WSAGetLastError());

	printf("Waiting for client connection....\n");

	addrLen = sizeof(sinClient);

	if ((clientSock = accept(serverSock, (struct sockaddr*)&sinClient, &addrLen)) == INVALID_SOCKET)
		ExitSys("accept", WSAGetLastError());

	printf("Connected: %s:%d\n", inet_ntoa(sinClient.sin_addr), /* to little endian*/ntohs(sinClient.sin_port));

	for (;;) {
		if ((result = recv(clientSock, buf, BUFFER_SIZE - 1, 0)) == SOCKET_ERROR)
			ExitSys("recv", WSAGetLastError());
		if (result == 0)
			break;

		buf[result] = '\0';
		if (!strcmp(buf, "exit"))
			break;
		printf("%d bytes received: %s\n", result, buf);
		_strrev(buf);
		if (send(clientSock, buf, strlen(buf), 0) == SOCKET_ERROR)
			ExitSys("send", WSAGetLastError());
	}

	shutdown(clientSock, SD_BOTH);
	closesocket(clientSock);
	closesocket(serverSock);

	WSACleanup();

	return 0;
}

void ExitSys(LPCSTR lpszMsg, DWORD dwLastErr)
{
	LPTSTR lpszErr;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL)) {
		fprintf(stderr, "%s: %s", lpszMsg, lpszErr);
		LocalFree(lpszErr);
	}

	exit(EXIT_FAILURE);
}