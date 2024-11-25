#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUFSIZE 1024
#define NICKNAMEMAXLEN 10
#define NICKNAMEMINLEN 3

#pragma comment(lib, "ws2_32.lib") // ws2_32.lib 링크

void ErrorHandling(char* message) {
    WSACleanup();
    fputs(message, stderr);
    exit(1);
}

int main(int argc, char** argv) {
    WSADATA wsaData;
    SOCKET servSock, clntSock;
    SOCKADDR_IN servAddr, clntAddr;
    char message[BUFSIZE];
    int strLen = 0, fromLen = 0, nRcv = 0, port = 0;
    char nickname[100];
    char opponentNickname[100];

    if (argc != 2) {
        printf("포트 번호를 입력해주세요. : ");
        scanf_s("%d", &port);
    }

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == INVALID_SOCKET)
        ErrorHandling("소켓 에러\n");

    memset(&servAddr, 0, sizeof(SOCKADDR_IN));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(argc == 2 ? atoi(argv[1]) : port);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("바인드 에러\n");

    if (listen(servSock, 2) == SOCKET_ERROR)
        ErrorHandling("리스닝 에러\n");

    fromLen = sizeof(clntAddr);

    clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &fromLen);
    if (clntSock == INVALID_SOCKET)
        ErrorHandling("연결 에러\n");
    else printf("연결 성공\n");

    while (1) {
        fflush(stdin);
        printf("닉네임을 입력해주세요. (최소 4자 , 최대 10자)\n : ");
        fflush(stdin);
        fgets(nickname, NICKNAMEMAXLEN, stdin);
        fflush(stdin);
        nickname[strcspn(nickname, "\n")] = '\0'; // 개행 문자 제거
        if ((int)strlen(nickname) < NICKNAMEMINLEN || (int)strlen(nickname) > NICKNAMEMAXLEN) {
            printf("닉네임이 올바르지 않습니다. 다시 입력해주세요\n");
        }
        else {
            printf("%s님이 대화방에 입장했습니다.\n", nickname);
            send(clntSock, nickname, (int)strlen(nickname), 0);
            break;
        }
    }

// 닉네임 입력 대기
        printf("상대방의 대화방 입장 기다리는 중...\n");
        nRcv = recv(clntSock, message, sizeof(message) - 1, 0);

        if (nRcv == SOCKET_ERROR) {
            printf("수신 에러..\n");
            return 0;
        }
        message[nRcv] = '\0';

        if (strcmp(message, "exit") == 0) {
            printf("클라이언트가 연결을 종료하였습니다.\n");
            return 0;
        }

        printf("%s님이 대화방에 입장했습니다.\n", message);
        // 상대방 닉네임 저장
        strcpy_s(opponentNickname, 100, message);

    while (1) {

        // 메세지 수신 대기
        nRcv = recv(clntSock, message, sizeof(message) - 1, 0);

        if (nRcv == SOCKET_ERROR) {
            printf("수신 에러..\n");
            break;
        }
        message[nRcv] = '\0';

        if (strcmp(message, "exit") == 0) {
            printf("클라이언트가 연결을 종료하였습니다.\n");
            break;
        }

        printf("받은 메시지 : %s\n", message);
        printf("보낼 메시지 : ");
        fgets(message, BUFSIZE, stdin);
        message[strcspn(message, "\n")] = '\0'; // 개행 문자 제거
        send(clntSock, message, (int)strlen(message), 0);
        if (strcmp(message, "exit") == 0) break;
    }

    closesocket(clntSock);
    WSACleanup();
    printf("연결 종료..\n");
    return 0;
}
