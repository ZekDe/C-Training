#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define SERVER_PORTNO			55000
#define BUFFER_SIZE				2048
#define MAX_PARAMS				32	

typedef struct tagCLIENT_REQUEST {
	char buf[BUFFER_SIZE];
	char* params[MAX_PARAMS + 1];
	int nparams;
} CLIENT_REQUEST;

typedef struct tagCLIENT_INFO {
	SOCKET sock;
	char addr[16];
	int port;
} CLIENT_INFO;

typedef struct tagUSER_INFO {
	char* userName;
	char* password;
} USER_INFO;

enum COMMANDS {
	LOGIN_COMMAND,
	ADD_COMMAND,
	SUB_COMMAND,
	MUL_COMMAND,
	DIV_COMMAND,
	LOGOUT_COMMAND,
};

static void ExitSys(LPCSTR lpszMsg, DWORD dwLastErr);
static DWORD __stdcall ClientThreadProc(LPVOID lpParam);
static int ReadLineSocket(SOCKET sock, char* buf, int size);
static int WriteSocket(SOCKET sock, const char* buf, int size);
static void ParseCommand(char* buf, CLIENT_REQUEST* clientRequest);
static BOOL ProcessCommand(const CLIENT_INFO* clientInfo, const CLIENT_REQUEST* clientRequest);

static BOOL LoginCommandProc(const CLIENT_REQUEST* clientRequest, char* buf);
static void OpCommandProc(const CLIENT_REQUEST* clientRequest, int commandCode, char* buf);
static void LogoutCommandProc(const CLIENT_REQUEST* clientRequest, char* buf);

USER_INFO g_users[] = {
	{"student", "12345"},
	{"kaan", "xxxxx"},
	{NULL, NULL}
};

void TCPIP_SimpleCalcServer_Example(void)
{
	WSADATA wsaData;
	DWORD dwResult;
	SOCKET serverSock, clientSock;
	struct sockaddr_in sinServer, sinClient;
	int addrLen;
	HANDLE hThread;
	DWORD dwThreadId;
	CLIENT_INFO* clientInfo;

	if ((dwResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
		ExitSys("WSAStartup", dwResult);

	if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 	/* third parameter IPPROTO_TCP */
		ExitSys("socket", WSAGetLastError());

	sinServer.sin_family = AF_INET;
	sinServer.sin_port = htons(SERVER_PORTNO);
	sinServer.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(serverSock, (struct sockaddr*)&sinServer, sizeof(sinServer)) == SOCKET_ERROR)
		ExitSys("bind", WSAGetLastError());

	if (listen(serverSock, 8) == SOCKET_ERROR)
		ExitSys("listen", WSAGetLastError());

	printf("Waiting for client connection....\n");
	for (;;) {
		addrLen = sizeof(sinClient);
		if ((clientSock = accept(serverSock, (struct sockaddr*)&sinClient, &addrLen)) == INVALID_SOCKET)
			ExitSys("accept", WSAGetLastError());

		if ((clientInfo = (CLIENT_INFO*)malloc(sizeof(CLIENT_INFO))) == NULL) {
			fprintf(stderr, "Cannot allocate memory!..\n");
			exit(EXIT_FAILURE);
		}
		clientInfo->sock = clientSock;
		strcpy(clientInfo->addr, inet_ntoa(sinClient.sin_addr));
		clientInfo->port = ntohs(sinClient.sin_port);

		printf("Connected client %s:%d\n", clientInfo->addr, clientInfo->port);

		if ((hThread = CreateThread(NULL, 0, ClientThreadProc, clientInfo, 0, &dwThreadId)) == NULL)
			ExitSys("CreateThread", WSAGetLastError());

		CloseHandle(hThread);
	}

	closesocket(serverSock);

	WSACleanup();
}

static DWORD __stdcall ClientThreadProc(LPVOID lpParam)
{
	CLIENT_INFO* clientInfo = (CLIENT_INFO*)lpParam;
	char buf[BUFFER_SIZE];
	int result;
	CLIENT_REQUEST clientRequest;

	for (;;) {
		if ((result = ReadLineSocket(clientInfo->sock, buf, BUFFER_SIZE)) == SOCKET_ERROR) {
			fprintf(stderr, "Error: ReadLineSocket, ClientConnection is closing...\n");
			break;
		}

		if (result == 0)
			break;
		printf("Client request: %s", buf);
		ParseCommand(buf, &clientRequest);
		if (!ProcessCommand(clientInfo, &clientRequest))
			break;
	}

	shutdown(clientInfo->sock, SD_BOTH);
	closesocket(clientInfo->sock);
	free(clientInfo);

	return 0;
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

static void ParseCommand(char* buf, CLIENT_REQUEST* clientRequest)
{
	char* str;
	int count;

	strcpy(clientRequest->buf, buf);
	for (count = 0, str = strtok(buf, " \t\n"); str != NULL && count < MAX_PARAMS; str = strtok(NULL, " \t\n"), ++count)
		clientRequest->params[count] = str;
	clientRequest->nparams = count;
}

static BOOL ProcessCommand(const CLIENT_INFO* clientInfo, const CLIENT_REQUEST* clientRequest)
{
	static char* clientRequests[] = { "LOGIN", "ADD", "SUB", "MUL", "DIV", "LOGOUT", NULL };
	int i;
	int result;
	char buf[BUFFER_SIZE];
	BOOL retVal = TRUE;

	if (clientRequest->nparams == 0) {
		sprintf(buf, "ERROR Empty command\n", clientRequest->params[0]);
		if ((result = WriteSocket(clientInfo->sock, buf, (int)strlen(buf))) == SOCKET_ERROR)
			fprintf(stderr, "Error: WriteSocket\n");
		return retVal;
	}

	for (i = 0; clientRequests[i] != NULL; ++i)
		if (!strcmp(clientRequests[i], clientRequest->params[0]))
			break;

	if (clientRequests[i] == NULL) {
		sprintf(buf, "ERROR Command not found \"%s\"\n", clientRequest->params[0]);
		if ((result = WriteSocket(clientInfo->sock, buf, (int)strlen(buf))) == SOCKET_ERROR)
			fprintf(stderr, "Error: WriteSocket\n");
		return retVal;
	}

	switch (i) {
	case LOGIN_COMMAND:
		if (!LoginCommandProc(clientRequest, buf)) {
			retVal = FALSE;
			break;
		}
		break;
	case ADD_COMMAND:
		OpCommandProc(clientRequest, ADD_COMMAND, buf);
		break;
	case SUB_COMMAND:
		OpCommandProc(clientRequest, SUB_COMMAND, buf);
		break;
	case MUL_COMMAND:
		OpCommandProc(clientRequest, MUL_COMMAND, buf);
		break;
	case DIV_COMMAND:
		OpCommandProc(clientRequest, DIV_COMMAND, buf);
		break;
	case LOGOUT_COMMAND:
		LogoutCommandProc(clientRequest, buf);
		retVal = FALSE;
		break;
	}
	if ((result = WriteSocket(clientInfo->sock, buf, (int)strlen(buf))) == SOCKET_ERROR) {
		fprintf(stderr, "Error: WriteSocket\n");
		retVal = FALSE;
	}

	return retVal;
}

static void OpCommandProc(const CLIENT_REQUEST* clientRequest, int commandCode, char* buf)
{
	double op1, op2, opResult;

	if (clientRequest->nparams == 3)
		if (sscanf(clientRequest->params[1], "%lf", &op1) == 1)
			if (sscanf(clientRequest->params[2], "%lf", &op2) == 1) {
				switch (commandCode) {
				case ADD_COMMAND:
					opResult = op1 + op2;
					break;
				case SUB_COMMAND:
					opResult = op1 - op2;
					break;
				case MUL_COMMAND:
					opResult = op1 * op2;
					break;
				case DIV_COMMAND:
					opResult = op1 / op2;
					break;
				}
				sprintf(buf, "RESULT %f\n", opResult);
			}
			else
				sprintf(buf, "ERROR Invalid command operand\n");
		else
			sprintf(buf, "ERROR Invalid command operand\n");
	else
		sprintf(buf, "ERROR Invalid command operand\n");
}

static BOOL LoginCommandProc(const CLIENT_REQUEST* clientRequest, char* buf)
{
	int i;

	for (i = 0; g_users[i].userName != NULL; ++i) {
		if (!strcmp(g_users[i].userName, clientRequest->params[1]) && !strcmp(g_users[i].password, clientRequest->params[2])) {
			sprintf(buf, "LOGIN_ACCEPTED\n");
			return TRUE;
		}
	}
	sprintf(buf, "ERROR Invalid user name or password\n");

	return FALSE;
}

static void LogoutCommandProc(const CLIENT_REQUEST* clientRequest, char* buf)
{
	if (clientRequest->nparams == 1)
		sprintf(buf, "LOGOUT_ACCEPTED Logout Accepted\n");
	else
		sprintf(buf, "ERROR Invalid command operand\n");
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