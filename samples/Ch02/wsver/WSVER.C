//
// WSVer.c
//
// WSAStartup()����Ԃ��������o�͂��܂��B
//

//
// �I�v�V�����Ƃ��āA�v���������o�[�W������
// �R�}���h���C���œn�����Ƃ��\�B
// ���W���[�o�[�W�����ԍ��ɑ�����
// �}�C�i�[�o�[�W�����ԍ���n�����ƁB
//
// ��F
// �o�[�W����1.1��v������ɂ�	WSVer 1 1
// �o�[�W����2.0��v������ɂ�	WSVer 2 0
//
// �������s����o�[�W�������w�肵�Ȃ��ꍇ�ɂ́A
// �v���O�����̓o�[�W����1.1��v�����܂��B
//

#include <stdio.h>
#include <winsock.h>

// Utility function in wsedesc.c
LPCSTR WSErrorDescription(int iErrorCode);

void PrintWSAData(LPWSADATA pWSAData);

void main(int argc, char **argv)
{
	WORD wVersionRequested = MAKEWORD(2,2);
	WSADATA wsaData;
	int rc;

	if (argc == 3)
		wVersionRequested = MAKEWORD(atol(argv[1]), 
								atol(argv[2]));

	printf("\nRequesting version %d.%d\n",
						LOBYTE(wVersionRequested),
						HIBYTE(wVersionRequested));

	rc = WSAStartup(wVersionRequested, &wsaData);
	if (!rc)
		PrintWSAData(&wsaData);
	else
		fprintf(stderr,"\nWSAStartup() error (%d) %s\n", 
										rc,
										WSErrorDescription(rc));
	WSACleanup();
}

void PrintWSAData(LPWSADATA pWSAData)
{
	printf("\nWSADATA");
	printf("\n----------------------");
	printf("\nVersion..............: %d.%d", 
			LOBYTE(pWSAData->wVersion),
			HIBYTE(pWSAData->wVersion));
	printf("\nHighVersion..........: %d.%d",
			LOBYTE(pWSAData->wHighVersion),
			HIBYTE(pWSAData->wHighVersion));
	printf("\nDescription..........: %s", 
			pWSAData->szDescription);
	printf("\nSystem status........: %s",
			pWSAData->szSystemStatus);
	printf("\nMax number of sockets: %d",
			pWSAData->iMaxSockets);
	printf("\nMAX UDP datagram size: %d\n",
			pWSAData->iMaxUdpDg);
}
