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
} CharInfo;
// 문자 -> 숫자 변환 함수
int charToNumeric(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 0; // 'a'는 0부터 시작
    }
    else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 0; // 'A'는 0부터 시작
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
    strcpy_s(binary, 9, temp); // 안전한 문자열 복사
}

// 8비트 이진수를 4비트씩 나누는 함수
// 8비트 이진수를 4비트씩 나누는 함수
void splitTo4Bits(char* binary, char* first4, char* second4) {
    strncpy_s(first4, 5, binary, 4);  // 첫 4비트 복사 (5는 배열 크기)
    strncpy_s(second4, 5, binary + 4, 4); // 다음 4비트 복사 (5는 배열 크기)
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

int main() {
    char input[100];
    printf("평문을 입력하세요: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // 개행 문자 제거

    // 현재 시간의 시와 분 합 계산
    int timeSum = getCurrentTime();
    char timeBinary[9];
    toBinary8Bit(timeSum, timeBinary);

    printf("현재 시간 (시+분) 합: %d, 8비트 이진수: %s\n", timeSum, timeBinary);

    // 입력받은 문자들을 저장할 구조체 배열
    CharInfo charInfos[100];
    int charCount = 0;

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

        // 숫자를 이진수로 변환
        toBinary8Bit(numeric, info.binary);

        // 이진수 XOR 결과 계산
        xorBinaryStrings(info.binary, timeBinary, info.xorResult);

        // XOR 결과를 Shift Row 수행
        strcpy_s(info.shifted, 9, info.xorResult);
        shiftRow(info.shifted, 2, 0); // 왼쪽으로 2비트 시프트

        // Shift Row 결과를 4비트씩 분리
        splitTo4Bits(info.shifted, info.first4Bits, info.second4Bits);

        // 4비트를 10진수로 변환
        info.first4Decimal = binaryToDecimal(info.first4Bits);
        info.second4Decimal = binaryToDecimal(info.second4Bits);

        // 구조체 배열에 저장
        charInfos[charCount++] = info;
    }

    // 결과 출력
    for (int i = 0; i < charCount; i++) {
        printf("문자: %c, 숫자: %d, 8비트 이진수: %s, XOR 결과: %s, Shift Row 결과: %s, 첫 4비트: %s (%d), 둘째 4비트: %s (%d)\n",
            charInfos[i].character,
            charInfos[i].numericValue,
            charInfos[i].binary,
            charInfos[i].xorResult,
            charInfos[i].shifted,
            charInfos[i].first4Bits,
            charInfos[i].first4Decimal,
            charInfos[i].second4Bits,
            charInfos[i].second4Decimal);
    }

    return 0;
}
