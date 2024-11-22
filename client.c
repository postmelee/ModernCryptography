#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define BUFSIZE 1024
  
void ErrorHandling(char *message) {
    WSACleanup();
    fputs(message, stderr);
    exit(1);
}
  
int main(int argc, char **argv) {
    SOCKET clntSock;
    WSADATA wsaData;
    SOCKADDR_IN servAddr;
    char sAddr[15];
    int sPort = 0, nRcv = 0;    
    unsigned int Addr;
    char message[BUFSIZE];   
    struct hostent *host;
	
    printf("서버 주소 : ");     
	scanf("%s", sAddr); 
    printf("포트 번호 : ");       
	scanf("%d", &sPort);
	fflush(stdin);
      
    WSAStartup(MAKEWORD(2, 2), &wsaData);
          
    clntSock = socket(AF_INET, SOCK_STREAM, 0);
    if(clntSock == INVALID_SOCKET)
        ErrorHandling("소켓 에러\n");
          
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(sAddr);
    servAddr.sin_port = htons(sPort);
  
    if(connect(clntSock, (void *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("연결 에러\n");
    else printf("연결 성공!\nStart...\n");
  
    while(1) {
        printf("\n보낼 메시지 : ");
        gets(message);
		fflush(stdin);
		
		send(clntSock, message, (int)strlen(message), 0);
        if(strcmp(message, "exit") == 0) break;
		
		printf("메시지 기다리는 중...\n");
        nRcv = recv(clntSock, message, sizeof(message) - 1, 0);
        if(nRcv == SOCKET_ERROR) {
            printf("수신 에러..\n");
            break;
        }
        message[nRcv] = '\0';
          
        if(strcmp(message, "exit") == 0) {
            printf("서버가 연결을 종료하였습니다.\n");
            break;
        }
		
		printf("받은 메시지 : %s", message);
		fflush(stdin);
    }
	
    closesocket(clntSock);
    WSACleanup();    
    printf("연결 종료..\n");
    return 0;            
}