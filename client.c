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
	
    printf("���� �ּ� : ");     
	scanf("%s", sAddr); 
    printf("��Ʈ ��ȣ : ");       
	scanf("%d", &sPort);
	fflush(stdin);
      
    WSAStartup(MAKEWORD(2, 2), &wsaData);
          
    clntSock = socket(AF_INET, SOCK_STREAM, 0);
    if(clntSock == INVALID_SOCKET)
        ErrorHandling("���� ����\n");
          
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(sAddr);
    servAddr.sin_port = htons(sPort);
  
    if(connect(clntSock, (void *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("���� ����\n");
    else printf("���� ����!\nStart...\n");
  
    while(1) {
        printf("\n���� �޽��� : ");
        gets(message);
		fflush(stdin);
		
		send(clntSock, message, (int)strlen(message), 0);
        if(strcmp(message, "exit") == 0) break;
		
		printf("�޽��� ��ٸ��� ��...\n");
        nRcv = recv(clntSock, message, sizeof(message) - 1, 0);
        if(nRcv == SOCKET_ERROR) {
            printf("���� ����..\n");
            break;
        }
        message[nRcv] = '\0';
          
        if(strcmp(message, "exit") == 0) {
            printf("������ ������ �����Ͽ����ϴ�.\n");
            break;
        }
		
		printf("���� �޽��� : %s", message);
		fflush(stdin);
    }
	
    closesocket(clntSock);
    WSACleanup();    
    printf("���� ����..\n");
    return 0;            
}