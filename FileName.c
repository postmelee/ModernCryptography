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
    char letter1;       // sum1을 알파벳으로 변환한 결과
    char letter2;       // sum2를 알파벳으로 변환한 결과
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
    {86, 87, 91, 92, 93, 94, 95, 96, 97, 98} };

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
    strcpy_s(binary, 9, temp); // 문자열 복사 (크기 지정)
}

// 8비트 이진수를 4비트씩 나누는 함수
void splitTo4Bits(char* binary, char* first4, char* second4) {
    strncpy_s(first4, 5, binary, 4);       // 첫 4비트 복사
    first4[4] = '\0';                      // 문자열 끝에 NULL 추가
    strncpy_s(second4, 5, binary + 4, 4);  // 다음 4비트 복사
    second4[4] = '\0';                     // 문자열 끝에 NULL 추가
}

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

int main() {
    char nickname[100];
    printf("닉네임을 입력하세요: ");
    fgets(nickname, sizeof(nickname), stdin);
    nickname[strcspn(nickname, "\n")] = '\0'; // 개행 문자 제거

    // 닉네임의 각 문자를 숫자로 변환
    int nicknameNumeric[100];
    int nicknameLength = strlen(nickname);
    for (int i = 0; i < nicknameLength; i++) {
        int numeric = charToNumeric(nickname[i]);
        if (numeric == -1) {
            printf("닉네임의 문자 '%c'는 변환할 수 없습니다.\n", nickname[i]);
            return 1; // 유효하지 않은 문자가 있으면 프로그램 종료
        }
        nicknameNumeric[i] = numeric;
    }

    char input[100];
    printf("평문을 입력하세요: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // 개행 문자 제거

    // 현재 시간의 시와 분 합 계산
    int timeSum = getCurrentTime();
    char timeBinary[9];

    // 두 자리 시간의 앞자리 - 행, 뒷자리 - 열로 테이블 조회
    int crytedTime = mixedTable[timeSum / 10][timeSum % 10];

    // 테이블을 통과한 숫자를 이진수로 변환
    toBinary8Bit(crytedTime, timeBinary);
    printf("현재 시간 (시+분) 합: %d, 테이블 통과한 시간값: %d, 테이블 통과한 시간값 이진수화: %s\n", timeSum, crytedTime, timeBinary);

    // 입력받은 문자들을 저장할 구조체 배열
    CharInfo charInfos[100];
    int charCount = 0;

    int nicknameIndex = 0; // 닉네임 인덱스 초기화

    // 평문의 각 문자 처리
    for (int i = 0; i < strlen(input); i++) {
        int numeric = charToNumeric(input[i]);
        if (numeric == -1) {
            printf("문자 '%c'는 변환할 수 없습니다.\n", input[i]);
            continue;
        }

        CharInfo info;
        info.character = input[i];
        info.numericValue = numeric;

        // 평문의 숫자를 이진수로 변환
        toBinary8Bit(numeric, info.binary);

        // 이진수 XOR 결과 계산
        xorBinaryStrings(info.binary, timeBinary, info.xorResult);

        // Shift Row 수행
        strcpy_s(info.shifted, 9, info.xorResult);
        shiftRow(info.shifted, 2, 0); // 왼쪽으로 2비트 시프트

        // Shift Row 결과를 4비트씩 분리
        splitTo4Bits(info.shifted, info.first4Bits, info.second4Bits);

        // 4x4 테이블 적용
        int firstDecimal = binaryToDecimal(info.first4Bits);
        int CfirstDecimal = fourByfourTable(firstDecimal);
        int secondDecimal = binaryToDecimal(info.second4Bits);
        int CsecondDecimal = fourByfourTable(secondDecimal);

        // 4비트를 10진수로 변환
        info.first4Decimal = CfirstDecimal;
        info.second4Decimal = CsecondDecimal;

        // 닉네임 숫자와 합산 후 mod 26
        info.sum1 = (info.first4Decimal + nicknameNumeric[nicknameIndex % nicknameLength]) % 26;
        nicknameIndex++;
        info.sum2 = (info.second4Decimal + nicknameNumeric[nicknameIndex % nicknameLength]) % 26;
        nicknameIndex++;

        // sum1과 sum2를 알파벳으로 변환
        info.letter1 = info.sum1 + 'A'; // 대문자로 변환
        info.letter2 = info.sum2 + 'A';

        // 구조체 배열에 저장
        charInfos[charCount++] = info;
    }

    // 결과 출력
    printf("\n닉네임: %s\n", nickname);
    printf("닉네임의 숫자 변환 결과: ");
    for (int i = 0; i < nicknameLength; i++) {
        printf("%d ", nicknameNumeric[i]);
    }
    printf("\n");

    // 암호화된 결과를 저장할 문자열
    char encryptedText[200] = "";
    for (int i = 0; i < charCount; i++) {
        printf("문자: %c, 숫자: %d, 8비트 이진수: %s, XOR 결과: %s, Shift Row 결과: %s\n", charInfos[i].character, charInfos[i].numericValue, charInfos[i].binary, charInfos[i].xorResult, charInfos[i].shifted);
        printf("첫 4비트: %s (%d), 둘째 4비트: %s (%d)\n", charInfos[i].first4Bits, charInfos[i].first4Decimal, charInfos[i].second4Bits, charInfos[i].second4Decimal);
        printf("합 (mod 26): sum1 = %d, sum2 = %d\n", charInfos[i].sum1, charInfos[i].sum2);
        printf("알파벳 변환: %c %c\n\n", charInfos[i].letter1, charInfos[i].letter2);

        // 암호화된 문자 추가
        int len = strlen(encryptedText);
        encryptedText[len] = charInfos[i].letter1;
        encryptedText[len + 1] = charInfos[i].letter2;
        encryptedText[len + 2] = '\0';
    }

    printf("암호화된 결과: %s\n", encryptedText);

    return 0;
}
