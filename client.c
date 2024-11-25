#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define BUFSIZE 1024
#define NICKNAMEMAXLEN 10
#define NICKNAMEMINLEN 3

#pragma comment(lib, "ws2_32.lib") // ws2_32.lib 링크

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
    char nickname[100]; 
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
    
    // 닉네임 입력
    while (1) {
        fflush(stdin);
        printf("닉네임을 입력해주세요. (최소 4자 , 최대 10자)\n 닉네임: ");
        fgets(nickname, NICKNAMEMAXLEN, stdin);
        fflush(stdin);
        nickname[strcspn(nickname, "\n")] = '\0'; // 개행 문자 제거
        if ((int)strlen(nickname) < NICKNAMEMINLEN || (int)strlen(nickname) > NICKNAMEMAXLEN) {
            printf("닉네임이 4자 이하입니다. 다시 입력해주세요\n");
        }
        else {
            char message[100];
            snprintf(message, sizeof(message), "%s님이 대화방에 입장했습니다.", nickname);
            printf("%s\n", message);
            send(clntSock, message, (int)strlen(message), 0);
            break;
        }
    }

    while(1) {
        // 상대방 닉네임 입력 대기
        printf("상대방의 대화방 입장 기다리는 중...\n");
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
		
		printf("%s\n", message);

        // 메세지 전송
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