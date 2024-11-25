#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// 암호화 함수
void encrypt(const char* plaintext, const char* key, char* ciphertext) {
    size_t textLen = strlen(plaintext);
    size_t keyLen = strlen(key);

    for (size_t i = 0, j = 0; i < textLen; i++) {
        char c = plaintext[i];

        if (isalpha(c)) {  // 알파벳 문자만 암호화
            char base = islower(c) ? 'a' : 'A';
            ciphertext[i] = (c - base + (key[j % keyLen] - '0')) % 26 + base;
            j++; // 키의 다음 문자로 이동
        }
        else {
            ciphertext[i] = c; // 알파벳이 아니면 그대로 저장
        }
    }
    ciphertext[textLen] = '\0'; // 문자열 종료
}

// 복호화 함수
void decrypt(const char* ciphertext, const char* key, char* plaintext) {
    size_t textLen = strlen(ciphertext);
    size_t keyLen = strlen(key);

    for (size_t i = 0, j = 0; i < textLen; i++) {
        char c = ciphertext[i];

        if (isalpha(c)) {  // 알파벳 문자만 복호화
            char base = islower(c) ? 'a' : 'A';
            plaintext[i] = (c - base - (key[j % keyLen] - '0') + 26) % 26 + base;
            j++; // 키의 다음 문자로 이동
        }
        else {
            plaintext[i] = c; // 알파벳이 아니면 그대로 저장
        }
    }
    plaintext[textLen] = '\0'; // 문자열 종료
}

// 현재 시간을 키로 변환
void generateKeyFromTime(char* key) {
    time_t currentTime;
    time(&currentTime);

    struct tm* localTime = localtime(&currentTime);

    // HHMM 형식으로 키 생성
    snprintf(key, 5, "%02d%02d", localTime->tm_hour, localTime->tm_min);
}

int main() {
    char plaintext[256];          // 사용자 입력 평문
    char key[5];                  // 시간 기반 키 (HHMM 형식)
    char ciphertext[256];
    char decrypted[256];

    // 평문 입력 받기
    printf("평문을 입력하세요: ");
    if (fgets(plaintext, sizeof(plaintext), stdin)) {
        plaintext[strcspn(plaintext, "\n")] = '\0'; // 개행 문자 제거
    }

    // 키 생성
    generateKeyFromTime(key);
    printf("생성된 키: %s\n", key);

    // 암호화
    encrypt(plaintext, key, ciphertext);
    printf("암호화된 텍스트: %s\n", ciphertext);

    // 복호화
    decrypt(ciphertext, key, decrypted);
    printf("복호화된 텍스트: %s\n", decrypted);

    return 0;
}
