#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_PORTNO			55000
#define CLIENT_PORTNO			62000
#define SERVER_NAME				"127.0.0.1"
#define BUFFER_SIZE				2048
#define MAX_USER_NAME			32
#define MAX_PASSWORD			32

typedef struct tagSERVER_RESPONSE {
	char* command;
	char* text;
} SERVER_RESPONSE;

enum COMMANDS {
	LOGIN_ACCEPTED_COMMAND,
	RESULT_COMMAND,
	ERROR_COMMAND,
	LOGOUT_ACCEPTED_COMMAND,
};

static int ReadLineSocket(SOCKET sock, char* buf, int size);
static int WriteSocket(SOCKET sock, const char* buf, int size);
static BOOL LoginProc(SOCKET clientSock, char* buf, SERVER_RESPONSE* serverResponse);
static void ParseCommand(char* buf, SERVER_RESPONSE* serverResponse);
static BOOL ProcessCommand(const SERVER_RESPONSE* serverResponse);
static void ExitSys(LPCSTR lpszMsg, DWORD dwLastErr);


void TCPIP_SimpleCalcClient_Example(void)
{
	WSADATA wsaData;
	DWORD dwResult;
	SOCKET clientSock;
	struct sockaddr_in sinServer;
	struct hostent* hostEnt;
	char buf[BUFFER_SIZE];
	int result;
	SERVER_RESPONSE serverResponse;

	if ((dwResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
		ExitSys("WSAStartup", dwResult);

	if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		ExitSys("socket", WSAGetLastError());
	/*
	{
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

	sinServer.sin_addr.s_addr = inet_addr(SERVER_NAME);
	if (sinServer.sin_addr.s_addr == INADDR_NONE) {
		if ((hostEnt = gethostbyname(SERVER_NAME)) == NULL)
			ExitSys("gethostbyname", WSAGetLastError());

		memcpy(&sinServer.sin_addr.s_addr, hostEnt->h_addr_list[0], hostEnt->h_length);
	}
	if (connect(clientSock, (struct sockaddr*)&sinServer, sizeof(sinServer)) == SOCKET_ERROR)
		ExitSys("connect", WSAGetLastError());

	printf("Connected...\n");

	if (!LoginProc(clientSock, buf, &serverResponse))
		goto FAILED;

	printf("Client connected...\n");

	for (;;) {
		printf("Calculator>");
		fflush(stdout);
		fgets(buf, BUFFER_SIZE, stdin);
		if ((result = WriteSocket(clientSock, buf, (int)strlen(buf))) == SOCKET_ERROR)
			ExitSys("WriteSocket", WSAGetLastError());
		if (result == 0)
			break;
		if ((result = ReadLineSocket(clientSock, buf, BUFFER_SIZE)) == SOCKET_ERROR)
			ExitSys("ReadLineSocket", WSAGetLastError());
		if (result == 0)
			break;

		ParseCommand(buf, &serverResponse);
		if (!ProcessCommand(&serverResponse))
			break;
	}
FAILED:
	shutdown(clientSock, SD_BOTH);
	closesocket(clientSock);

	WSACleanup();
}

static int ReadLineSocket(SOCKET sock, char* buf, int size)
{
	char* bufx = buf;
	static char* bp;
	static int count = 0;
	static char b[2048];
	char ch;

	while (--size > 0) {
		if (--count <= 0) {
			count = recv(sock, b, sizeof(b), 0);
			if (count == SOCKET_ERROR)
				return SOCKET_ERROR;
			if (count == 0)
				return 0;
			bp = b;
		}
		ch = *bp++;
		*buf++ = ch;
		if (ch == '\n') {
			*buf = '\0';
			return (int)(buf - bufx);
		}
	}

	return SOCKET_ERROR;
}

static int WriteSocket(SOCKET sock, const char* buf, int size)
{
	int left, count;
	int result;

	left = size;
	count = 0;

	while (left > 0) {
		if ((result = send(sock, buf + count, left, 0)) == SOCKET_ERROR)
			return SOCKET_ERROR;
		if (result == 0)
			break;
		left -= result;
		count += result;
	}

	return count;
}

static BOOL LoginProc(SOCKET clientSock, char* buf, SERVER_RESPONSE* serverResponse)
{
	char userName[MAX_USER_NAME];
	char password[MAX_PASSWORD];

	int result;
	char* str;

	printf("User name:");
	fgets(userName, MAX_USER_NAME, stdin);
	if ((str = strchr(userName, '\n')) != NULL)
		*str = '\0';
	printf("Password:");
	fgets(password, MAX_PASSWORD, stdin);
	if ((str = strchr(password, '\n')) != NULL)
		*str = '\0';

	sprintf(buf, "LOGIN %s %s\n", userName, password);

	if ((result = WriteSocket(clientSock, buf, (int)strlen(buf))) == SOCKET_ERROR || result == 0) {
		fprintf(stderr, "Error WriteSocket!..\n");
		return FALSE;
	}


	if ((result = ReadLineSocket(clientSock, buf, BUFFER_SIZE)) == SOCKET_ERROR || result == 0) {
		fprintf(stderr, "Error ReadLineSocket!..\n");
		return FALSE;
	}

	ParseCommand(buf, serverResponse);

	if (strcmp(serverResponse->command, "LOGIN_ACCEPTED") != 0) {
		printf("%s\n", serverResponse->text);
		return FALSE;
	}

	return TRUE;
}

static void ParseCommand(char* buf, SERVER_RESPONSE* serverResponse)
{
	char* str = buf;

	str = buf;
	serverResponse->command = str;

	while (*buf != ' ' && *buf != '\n')
		++buf;
	if (*buf == '\n') {
		*buf = '\0';
		serverResponse->text = NULL;
		return;
	}
	*buf++ = '\0';

	str = buf;
	while (*buf != '\n')
		++buf;
	*buf = '\0';
	serverResponse->text = str;
}

static BOOL ProcessCommand(const SERVER_RESPONSE* serverResponse)
{
	static char* serverCommands[] = { "LOGIN_ACCEPTED", "RESULT", "ERROR", "LOGOUT_ACCEPTED", NULL };
	int i;
	BOOL retVal = TRUE;

	for (i = 0; serverCommands[i] != NULL; ++i)
		if (!strcmp(serverCommands[i], serverResponse->command))
			break;

	switch (i) {
	case RESULT_COMMAND:
	case ERROR_COMMAND:
		printf("%s\n", serverResponse->text);
		break;
	case LOGOUT_ACCEPTED_COMMAND:
		printf("%s\n", serverResponse->text);
		retVal = FALSE;
		break;
	default:
		printf("Unknown server command: %s\n", serverResponse->text);
	}

	return retVal;
}

static void ExitSys(LPCSTR lpszMsg, DWORD dwLastErr)
{
	LPTSTR lpszErr;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL)) {
		fprintf(stderr, "%s: %s", lpszMsg, lpszErr);
		LocalFree(lpszErr);
	}

	exit(EXIT_FAILURE);
}
