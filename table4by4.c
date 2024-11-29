#include <stdio.h>

int main() {
    // 고정된 랜덤 4x4 테이블 정의
    int table[4][4] = {
        {3, 7, 11, 2},
        {14, 6, 9, 1},
        {12, 0, 5, 8},
        {10, 13, 4, 15}
    };

    // 역테이블 생성 (크기 16, 값의 범위 0-15)
    int reverseTable[16][2];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int value = table[i][j];
            reverseTable[value][0] = i; // 행
            reverseTable[value][1] = j; // 열
        }
    }

    // 4비트 2진수 입력 받기
    int binaryInput;
    printf("4비트 2진수를 입력하세요 (예: 1101): ");
    scanf("%d", &binaryInput);

    // 입력된 2진수를 행과 열로 변환
    int row = (binaryInput >> 2) & 0x3; // 첫 2비트
    int col = binaryInput & 0x3;        // 뒤 2비트

    // 테이블에서 값 찾기
    int value = table[row][col];

    // 역테이블을 사용하여 디코딩된 위치 찾기
    int decodedRow = reverseTable[value][0];
    int decodedCol = reverseTable[value][1];

    // 디코딩된 위치를 4비트 2진수로 변환
    int decodedBinary = (decodedRow << 2) | decodedCol;

    // 결과 출력
    printf("입력된 위치의 10진수 값: %d\n", value);
    printf("디코딩된 4비트 2진수: %04d\n", decodedBinary);

    return 0;
}