#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <time.h>

// 문자 정보를 저장하는 구조체 정의
typedef struct {
    char character;     // 문자
    int numericValue;   // 숫자 값
    char binary[9];     // 8비트 이진수 문자열
    char xorResult[9];  // XOR 결과 이진수 문자열
    char shifted[9];    // Shift Row 결과
    char first4Bits[5]; // 첫 4비트
    char second4Bits[5];// 둘째 4비트
    int first4Decimal;  // 첫 4비트의 10진수 값
    int second4Decimal; // 둘째 4비트의 10진수 값
    int sum1;           // 첫 합 (first4Decimal + 닉네임 숫자) mod 26
    int sum2;           // 둘째 합 (second4Decimal + 닉네임 숫자) mod 26
    char letter1;       // 알파벳으로 변환한 결과
    char letter2;       // 알파벳으로 변환한 결과
    char* result; // 비트화된 결과
} CharInfo;

// 섞인 테이블
int mixedTable[10][10] = {
    {12, 5, 8, 19, 23, 45, 67, 89, 34, 56},
    {78, 90, 11, 22, 33, 44, 55, 66, 77, 88},
    {99, 0, 1, 2, 3, 4, 6, 7, 9, 10},
    {13, 14, 15, 16, 17, 18, 20, 21, 24, 25},
    {26, 27, 28, 29, 30, 31, 32, 35, 36, 37},
    {38, 39, 40, 41, 42, 43, 46, 47, 48, 49},
    {50, 51, 52, 53, 54, 57, 58, 59, 60, 61},
    {62, 63, 64, 65, 68, 69, 70, 71, 72, 73},
    {74, 75, 76, 79, 80, 81, 82, 83, 84, 85},
    {86, 87, 91, 92, 93, 94, 95, 96, 97, 98}
};

// 문자 -> 숫자 변환 함수
int charToNumeric(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a'; // 'a'는 0부터 시작
    }
    else if (c >= 'A' && c <= 'Z') {
        return c - 'A'; // 'A'는 0부터 시작
    }
    else {
        return -1; // 유효하지 않은 문자 처리
    }
}

// 정수를 8비트 이진수 문자열로 변환하는 함수
void toBinary8Bit(int num, char* binary) {
    for (int i = 7; i >= 0; i--) {
        binary[i] = (num % 2) + '0'; // 나머지를 이용하여 이진수 계산
        num /= 2;
    }
    binary[8] = '\0'; // 문자열 끝에 NULL 추가
}

// 정수를 4비트 이진수 문자열로 변환하는 함수
void toBinary4Bit(int num, char* binary) {
    for (int i = 3; i >= 0; i--) {
        binary[i] = (num % 2) + '0'; // 나머지를 이용하여 이진수 계산
        num /= 2;
    }
    binary[4] = '\0'; // 문자열 끝에 NULL 추가
}

// 두 개의 8비트 이진수 문자열을 XOR 연산하는 함수
void xorBinaryStrings(char* binary1, char* binary2, char* result) {
    for (int i = 0; i < 8; i++) {
        // 각 비트를 정수로 변환하여 XOR 연산 후, 다시 문자로 변환
        result[i] = ((binary1[i] - '0') ^ (binary2[i] - '0')) + '0';
    }
    result[8] = '\0'; // 문자열 끝에 NULL 추가
}

// 8비트 이진수를 순환 시프트하는 함수
void shiftRow(char* binary, int shiftAmount, int direction) {
    char temp[9]; // 임시 저장 배열
    int len = 8;  // 8비트 길이 고정

    if (direction == 0) { // 왼쪽 순환 시프트
        for (int i = 0; i < len; i++) {
            temp[i] = binary[(i + shiftAmount) % len];
        }
    }
    else if (direction == 1) { // 오른쪽 순환 시프트
        for (int i = 0; i < len; i++) {
            temp[i] = binary[(i - shiftAmount + len) % len];
        }
    }

    temp[len] = '\0'; // NULL 문자 추가
    strcpy(binary, temp); // 문자열 복사
}

// 8비트 이진수를 4비트씩 나누는 함수
void splitTo4Bits(char* binary, char* first4, char* second4) {
    strncpy(first4, binary, 4);       // 첫 4비트 복사
    first4[4] = '\0';                      // 문자열 끝에 NULL 추가
    strncpy(second4, binary + 4, 4);  // 다음 4비트 복사
    second4[4] = '\0';                     // 문자열 끝에 NULL 추가
}

// 
int binaryToDecimal(char* binary) {
    int decimal = 0;
    int length = strlen(binary);
    for (int i = 0; i < length; i++) {
        decimal = decimal * 2 + (binary[i] - '0');
    }
    return decimal;
}

// 현재 시간을 가져와 시와 분의 합을 반환하는 함수
int getCurrentTime() {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    return local->tm_hour + local->tm_min;
}

void join4BitsTo8Bit (char* result, char* first4Bits, char* second4Bits) {
    strcpy(result, first4Bits);
    strcat(result, second4Bits);
    return;
}

// 4x4 테이블 함수
int fourByfourTable(int binaryInput) {
    // 고정된 랜덤 4x4 테이블 정의
    int table[4][4] = {
        {3, 7, 11, 2},
        {14, 6, 9, 1},
        {12, 0, 5, 8},
        {10, 13, 4, 15}
    };

    // 입력된 2진수를 행과 열로 변환
    int row = (binaryInput >> 2) & 0x3; // 첫 2비트
    int col = binaryInput & 0x3;        // 뒤 2비트

    return table[row][col];
}

// 역 4x4 테이블 함수
int reverseFourByfourTable(int outputValue) {
    // 고정된 랜덤 4x4 테이블 정의
    int table[4][4] = {
        {3, 7, 11, 2},
        {14, 6, 9, 1},
        {12, 0, 5, 8},
        {10, 13, 4, 15}
    };

    for (int input = 0; input < 16; input++) {
        int row = (input >> 2) & 0x3;
        int col = input & 0x3;
        if (table[row][col] == outputValue) {
            return input;
        }
    }
    // 만약 찾지 못했다면 -1 반환
    return -1;
}

void nicknameToNumeric(const char* nickname, int result[]) {
    int nicknameLength = strlen(nickname);
    for (int i = 0; i < nicknameLength; i++) {
        int numeric = charToNumeric(nickname[i]);
        if (numeric == -1) {
            printf("닉네임의 문자 '%c'는 변환할 수 없습니다.\n", nickname[i]);
            return;
        }
        result[i] = numeric;
    }
}
// 인코딩 함수
void encodeMessage(const char* nickname, const char* plaintext, char* encryptedText) {

    int nicknameNumeric[100];
    int nicknameLength = strlen(nickname);
    for (int i = 0; i < nicknameLength; i++) {
        int numeric = charToNumeric(nickname[i]);
        if (numeric == -1) {
            printf("닉네임의 문자 '%c'는 변환할 수 없습니다.\n", nickname[i]);
            encryptedText[0] = '\0';  // 에러 처리
            return;
        }
        nicknameNumeric[i] = numeric;
    }
    // 현재 시간의 시와 분 합 계산
    int timeSum = getCurrentTime();
    char timeBinary[9];

    // 두 자리 시간의 앞자리 - 행, 뒷자리 - 열로 테이블 조회 (10x10 테이블)
    int crytedTime = mixedTable[(timeSum / 10) % 10][timeSum % 10];

    // 테이블을 통과한 숫자를 이진수로 변환
    toBinary8Bit(crytedTime, timeBinary);

    // 입력받은 문자들을 저장할 구조체 배열
    CharInfo charInfos[100];
    int charCount = 0;

    // 평문의 각 문자 처리
    for (int i = 0; i < strlen(plaintext); i++) {
        int numeric = charToNumeric(plaintext[i]);
        if (numeric == -1) {
            printf("문자 '%c'는 변환할 수 없습니다.\n", plaintext[i]);
            continue;
        }

        CharInfo info;
        info.character = plaintext[i];
        info.numericValue = numeric;

        // 평문의 숫자를 이진수로 변환
        toBinary8Bit(numeric, info.binary);

        // [plaintext와 시간] XOR 결과 계산
        char result[9];
        xorBinaryStrings(info.binary, timeBinary, result);


        // Round 시작

        int roundNum = 3;



        for(int round=0; round<roundNum; round++) {
            int nicknameIndex = 0; // 닉네임 인덱스 초기화
            // Shift Row 수행
            strcpy(info.shifted, result);
            shiftRow(info.shifted, 2, 0); // 왼쪽으로 2비트 시프트
            // Shift Row 결과를 4비트씩 분리
            splitTo4Bits(info.shifted, info.first4Bits, info.second4Bits);

            // 4비트 -> 10진수 변환
            int firstDecimal = binaryToDecimal(info.first4Bits);
            int secondDecimal = binaryToDecimal(info.second4Bits);

            // 4x4 테이블 적용, 
            info.first4Decimal = fourByfourTable(firstDecimal);
            info.second4Decimal = fourByfourTable(secondDecimal);

                // 버지니아 

            // 닉네임 숫자와 합산 후 mod 26 (버지니아)
            int nicknameLength = strlen(nickname);
            info.sum1 = (info.first4Decimal + nicknameNumeric[nicknameIndex % nicknameLength]) % 16;
            nicknameIndex++;
            info.sum2 = (info.second4Decimal + nicknameNumeric[nicknameIndex % nicknameLength]) % 16;
            nicknameIndex++;
            // 위 결과를 2진수 4비트화
            char first4Bits[5]; char second4Bits[5];
            toBinary4Bit(info.sum1, first4Bits);
            toBinary4Bit(info.sum2, second4Bits);
            join4BitsTo8Bit(result, first4Bits, second4Bits);

            // printf("Round %d: %s\n", round, result);
        }
        // 전송
        // sum1과 sum2를 알파벳으로 변환
        char firstResult4Bits[5]; char secondResult4Bits[5];

        splitTo4Bits(result, firstResult4Bits, secondResult4Bits);

        info.letter1 = binaryToDecimal(firstResult4Bits) + 'A'; // 대문자로 변환
        info.letter2 = binaryToDecimal(secondResult4Bits) + 'A'; 
        info.result = result;
        // 구조체 배열에 저장
        charInfos[charCount++] = info;
    }

    // 암호화된 결과를 저장할 문자열
    encryptedText[0] = '\0'; // 초기화
    for (int i = 0; i < charCount; i++) {
        int len = strlen(encryptedText);
        encryptedText[len] = charInfos[i].letter1;
        encryptedText[len + 1] = charInfos[i].letter2;
        encryptedText[len + 2] = '\0';
    }

    printf("사용된 시간 합: %d\n", timeSum); // 디버깅을 위해 시간 합 출력
}

// 디코딩 함수
void decodeMessage(const char* nickname, const char* encryptedText, char* decryptedText, int timeSum) {
    // 닉네임의 각 문자를 숫자로 변환
    int nicknameNumeric[100];
    int nicknameLength = strlen(nickname);
    for (int i = 0; i < nicknameLength; i++) {
        int numeric = charToNumeric(nickname[i]);
        if (numeric == -1) {
            printf("닉네임의 문자 '%c'는 변환할 수 없습니다.\n", nickname[i]);
            decryptedText[0] = '\0'; // 에러 처리
            return;
        }
        nicknameNumeric[i] = numeric;
    }

    // 시간 합으로부터 timeBinary 생성
    char timeBinary[9];
    int crytedTime = mixedTable[(timeSum / 10) % 10][timeSum % 10];
    toBinary8Bit(crytedTime, timeBinary);

    int decryptedIndex = 0;

    // 암호문의 각 문자 쌍 처리
    for (int i = 0; i < strlen(encryptedText); i+=2) {
        int roundNum = 3;


        int sum1 = charToNumeric(encryptedText[i]);
        int sum2 = charToNumeric(encryptedText[i+1]);

        char firstSum4Bits[5]; char secondSumBits[5];
        toBinary4Bit(sum1, firstSum4Bits);
        toBinary4Bit(sum2, secondSumBits);

        char roundResult[9];
        join4BitsTo8Bit(roundResult, firstSum4Bits, secondSumBits);

        // Round start
        for (int round = 0; round<roundNum; round++) {
            int nicknameIndex = 0; // 닉네임 인덱스 초기화
            // 문자를 10진수로 변환, 찢기
            char roundFirst4Bits[5]; char roundSecond4Bits[5];
            splitTo4Bits(roundResult, roundFirst4Bits, roundSecond4Bits);
            int sum1 = binaryToDecimal(roundFirst4Bits);
            int sum2 = binaryToDecimal(roundSecond4Bits);

            // 닉네임 숫자와의 역연산 (버지니아)
            int CfirstDecimal = (sum1 - nicknameNumeric[nicknameIndex % nicknameLength] + 32) % 16;
            nicknameIndex++;
            int CsecondDecimal = (sum2 - nicknameNumeric[nicknameIndex % nicknameLength] + 32) % 16;
            nicknameIndex++;

            // 4x4 테이블의 역변환
            int firstDecimal = reverseFourByfourTable(CfirstDecimal);
            int secondDecimal = reverseFourByfourTable(CsecondDecimal);

            if (firstDecimal == -1 || secondDecimal == -1) {
                printf("4x4 테이블에서 역변환할 수 없습니다.\n");
                continue;
            }

            // 4비트 이진수로 변환
            char first4Bits[5], second4Bits[5];
            toBinary4Bit(firstDecimal, first4Bits);
            toBinary4Bit(secondDecimal, second4Bits);
            
            // 이진수 결합
            char shifted[9];
            join4BitsTo8Bit(shifted, first4Bits, second4Bits);

            // 순환 시프트 복원 (오른쪽으로 2비트)
            strcpy(roundResult, shifted);
            shiftRow(roundResult, 2, 1); // 오른쪽으로 2비트 시프트
        }

        // XOR 연산 복원
        char binary[9];
        xorBinaryStrings(roundResult, timeBinary, binary);

        // 이진수를 10진수로 변환
        int numericValue = binaryToDecimal(binary);
        
        // 숫자를 문자로 변환
        char character = numericValue + 'a'; // 대문자로 변환

        decryptedText[decryptedIndex++] = character;
    }
    decryptedText[decryptedIndex] = '\0'; // 문자열 끝에 NULL 추가
}

void encryptTransmissionTime(int transmissionTime, int serverTime, char* encryptedTime) {
    // 전송 시간과 서버 시간을 XOR 연산하여 암호화
    int encryptedValue = transmissionTime ^ serverTime;
    sprintf(encryptedTime, "%08X", encryptedValue); // 16진수 문자열로 변환
}

int main() {
    char nickname[100];
    printf("닉네임을 입력하세요: ");
    fgets(nickname, sizeof(nickname), stdin);
    nickname[strcspn(nickname, "\n")] = '\0'; // 개행 문자 제거

    char plaintext[100];
    printf("평문을 입력하세요: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // 개행 문자 제거

    char encryptedText[200];
    encodeMessage(nickname, plaintext, encryptedText);

        // 전송 시간 추가 (분 단위)
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    int transmissionTime = local->tm_hour * 60 + local->tm_min; // 현재 시간을 분 단위로 획득
    char transmissionTimeStr[20];
    sprintf(transmissionTimeStr, "%d", transmissionTime);

    // 서버 접속 시간 획득 (분 단위)
    now = time(NULL);
    local = localtime(&now);
    int serverTime = local->tm_hour * 60 + local->tm_min; // 서버 접속 시간을 분 단위로 획득

    // 전송 시간 암호화
    char encryptedTransmissionTime[20];
    encryptTransmissionTime(transmissionTime, serverTime, encryptedTransmissionTime);

    // 암호화된 전송 시간 추가
    // strcat(encryptedText, "|"); // 구분자 추가
    // strcat(encryptedText, encryptedTransmissionTime);

    printf("암호화된 결과: %s\n", encryptedText);

    // 디코딩 과정
    char decryptedText[100];
    int timeSum;
    printf("암호화에 사용된 시간 합을 입력하세요: ");
    scanf("%d", &timeSum);

     // 암호문과 전송 시간 분리
    // char* encryptedMessage = strtok(encryptedText, "|");
    // char* encryptedTime = strtok(NULL, "|");


    decodeMessage(nickname, encryptedText, decryptedText, timeSum);

    printf("복호화된 결과: %s\n", decryptedText);

    return 0;
}