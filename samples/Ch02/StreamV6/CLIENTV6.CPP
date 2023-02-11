//
// ClientV6.cpp
//
// ����߂ĒP���ȃX�g���[���N���C�A���g��B
// ServerV6.cpp�Ƃ̘A�g�œ��삵�܂��B
//
// �v���O�����́A�R�}���h���C���Ŏw�肵���T�[�o�[�ƃ|�[�g�ɐڑ������݂܂��B
// ServerV6�v���O�����͋N�����ɕK�v�ȏ����o�͂��܂��B
// �ڑ�����ƁA�v���O�����̓T�[�o�[�փf�[�^�𑗐M���A
// ������҂��A�I�����܂��B
//
// ws2_32.lib�ŃR���p�C���E�����N���܂��B
//
// IPv6�Ńe�X�g����ɂ́A�܂�Windows��IPv6�X�^�b�N��
// �C���X�g�[������K�v������܂��B
// Windows XP��IPv6���C���X�g�[������ɂ́A
// �R�}���h�v�����v�g���J���A���L�̂悤�ɓ��͂��܂��B
//
// ipv6 install
//
// �g�p���@�F
//
// ClientV6 ServerName portnumber bIPv6
//
// 1�Ԗڂ̃p�����[�^ServerName�́A�T�[�o�[���B
// 
// 2�Ԗڂ̃p�����[�^portnumber�́A�T�[�o�[��bind()�����|�[�g�ԍ��B 
// 
// 3�Ԗڂ̃p�����[�^bIPv6�́AIPv6�g�p����1�AIPv4�g�p����0�B
//
// ��F ClientV6 ServerName 2000 1
//

#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>

// Function prototype
void StreamClient(char *szServer, char* pszPort, char *ipvNum);

// Helper macro for displaying errors
#define PRINTERROR(s)	\
		fprintf(stderr,"\n%s: %d\n", s, WSAGetLastError())

////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;

	//
	// Check for the host and port and IP version arguments
	//
	if (argc != 4)
	{
		fprintf(stderr,"\nSyntax: client ServerName PortNumber bIPv6\n");
		return;
	}

	//
	// Initialize WinSock and check the version
	//
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		fprintf(stderr,"\n Wrong version\n");
		return;
	}


	//
	// Go do the stuff a stream client does
	//
	StreamClient(argv[1], argv[2], argv[3]);

	
	//
	// Release WinSock
	//
	WSACleanup();
}

////////////////////////////////////////////////////////////

void StreamClient(char *szServer, char* pszPort, char *pszIPV)
{
	int ipvVer;
	int aiFamily;

	if(strcmp(pszIPV, "0") == 0){
		ipvVer = 4;	
		aiFamily = AF_INET;
	}else if(strcmp(pszIPV, "1") == 0){
		ipvVer = 6;
		aiFamily = AF_INET6;
	}else{
		printf("\nIP version 1 = IPv6 or 0 = IPv4\n");
		return;
	}

	printf("\nStream Client connecting to server: %s on port: %s using ipv: %d",
				szServer, pszPort, ipvVer);
	ADDRINFO Hints, *AI;
	int nRet;

    memset(&Hints, 0, sizeof(Hints));
    Hints.ai_family = aiFamily; 
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_flags = 0;

    nRet = getaddrinfo(szServer, pszPort, &Hints, &AI);
	if( nRet != 0 ){
        fprintf(stderr, "getaddrinfo failed with error %d: %s\n", nRet, gai_strerror(nRet));
        return;
    }

	//
	// Create a TCP/IP stream socket
	//
	SOCKET	theSocket;

	theSocket = socket(AI->ai_family,			// Address family
					   SOCK_STREAM,				// Socket type
					   0);						// Protocol
	if (theSocket == INVALID_SOCKET)
	{
		PRINTERROR("socket()");
		return;
	}

	nRet = connect(theSocket,				// Socket
				   AI->ai_addr,				// Server address
				   (int)AI->ai_addrlen);			// Length of server address structure
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("socket()");
		closesocket(theSocket);
		return;
	}

	freeaddrinfo( AI );

	//
	// Send data to the server
	//
	char szBuf[256];

	strcpy(szBuf, "From the Client");
	nRet = send(theSocket,				// Connected socket
				szBuf,					// Data buffer
				(int)strlen(szBuf),			// Length of data
				0);						// Flags
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("send()");
		closesocket(theSocket);
		return;
	}


	//
	// Wait for a reply
	//
	nRet = recv(theSocket,				// Connected socket
				szBuf,					// Receive buffer
				sizeof(szBuf),			// Size of receive buffer
				0);						// Flags
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("recv()");
		closesocket(theSocket);
		return;
	}


	//
	// Display the received data
	//
	printf("\nData received: %s", szBuf);


	closesocket(theSocket);
	return;
}