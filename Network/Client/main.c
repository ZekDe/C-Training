#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_PORTNO			55000
#define CLIENT_PORTNO			62000
#define BUFFER_SIZE				1024
#define SERVER_NAME				"127.0.0.1" // or name(localhost this machine)

void ExitSys(LPCSTR lpszMsg, DWORD dwLastErr);

int main(void)
{
	WSADATA wsaData;
	DWORD dwResult;
	SOCKET clientSock;
	struct sockaddr_in sinServer;
	struct hostent* hostEnt;
	char buf[BUFFER_SIZE];
	char* str;
	int result;

	if ((dwResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
		ExitSys("WSAStartup", dwResult);

	if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		ExitSys("socket", WSAGetLastError());
	/*
	{
	// system assign a port automatically 
		struct sockaddr_in sinClient;

		sinClient.sin_family = AF_INET;
		sinClient.sin_port = htons(CLIENT_PORTNO);
		sinClient.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(clientSock, (struct sockaddr *)&sinClient, sizeof(sinClient)) == SOCKET_ERROR)
			ExitSys("bind", WSAGetLastError());
	}
	*/

	sinServer.sin_family = AF_INET;
	sinServer.sin_port = htons(SERVER_PORTNO);

	sinServer.sin_addr.s_addr = inet_addr(SERVER_NAME);// if SERVER_NAME is dotted decimal format, 
	//convert it to numbers, if not, result none and start DNS 
	if (sinServer.sin_addr.s_addr == INADDR_NONE) { // check DNS is success
		// get ip address via hostEnd
		if ((hostEnt = gethostbyname(SERVER_NAME)) == NULL) 
			ExitSys("gethostbyname", WSAGetLastError());
		//hostEnt->h_aliases: alternative names

		memcpy(&sinServer.sin_addr.s_addr, hostEnt->h_addr_list[0], hostEnt->h_length);
	}
	if (connect(clientSock, (struct sockaddr*)&sinServer, sizeof(sinServer)) == SOCKET_ERROR)
		ExitSys("connect", WSAGetLastError());

	printf("Connected...\n");
	for (;;) {
		printf("Text:");

		fgets(buf, BUFFER_SIZE, stdin);
		if ((str = strchr(buf, '\n')) != NULL)
			*str = '\0';

		if (send(clientSock, buf, strlen(buf), 0) == SOCKET_ERROR)
			ExitSys("send", WSAGetLastError());

		if (!strcmp(buf, "exit"))
			break;

		if ((result = recv(clientSock, buf, BUFFER_SIZE - 1, 0)) == SOCKET_ERROR)
			ExitSys("recv", WSAGetLastError());
		if (result == 0)
			break;

		buf[result] = '\0';

		printf("%d bytes received: %s\n", result, buf);
	}

	shutdown(clientSock, SD_BOTH);
	closesocket(clientSock);

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
