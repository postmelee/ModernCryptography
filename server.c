#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUFSIZE 1024
  
void ErrorHandling(char *message) {
    WSACleanup();
    fputs(message, stderr);
    exit(1);
}
  
int main(int argc, char **argv) {
    WSADATA wsaData;
    SOCKET servSock, clntSock;
    SOCKADDR_IN servAddr, clntAddr;
    char message[BUFSIZE];
    int strLen = 0, fromLen = 0, nRcv = 0, port = 0;
      
    if(argc!=2) {
        printf("포트 번호를 입력해주세요. : ");
		scanf("%d", &port);
    }
	
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	
    servSock = socket(PF_INET, SOCK_STREAM, 0);
    if(servSock == INVALID_SOCKET)
        ErrorHandling("소켓 에러\n");
	
    memset(&servAddr, 0, sizeof(SOCKADDR_IN));
    servAddr.sin_family = AF_INET;
	if	(argc == 2)	servAddr.sin_port = htons(atoi(argv[1]));
    else			servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      
    if(bind(servSock, (void *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("바인드 에러\n");
	
    if(listen(servSock, 2) == SOCKET_ERROR)
        ErrorHandling("리스닝 에러\n");        
	
    fromLen = sizeof(clntAddr);
      
    clntSock = accept(servSock, (void *)&clntAddr, &fromLen);
    if(clntSock == INVALID_SOCKET)
        ErrorHandling("연결 에러\n");
    else printf("%s 연결 성공!\nStart ...\n", inet_ntoa(clntAddr.sin_addr));
	
    while(1) {
		printf("메시지 기다리는 중...\n");
        nRcv = recv(clntSock, message, sizeof(message) - 1, 0);
          
        if(nRcv == SOCKET_ERROR) {
            printf("수신 에러..\n");
            break;
        }
        message[nRcv] = '\0';
          
        if(strcmp(message, "exit") == 0) {
            printf("클라이언트가 연결을 종료하였습니다.\n");
            break;
        }
          
        printf("받은 메시지 : %s", message);
		fflush(stdin);
        printf("\n보낼 메시지 : ");
        gets(message);
		fflush(stdin);
        send(clntSock, message, (int)strlen(message), 0); 
        if(strcmp(message, "exit") == 0) break;
    }
	
    closesocket(clntSock);
    WSACleanup();
    printf("연결 종료..\n");
    return 0;    
}