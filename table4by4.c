#include <stdio.h>

int main() {
    // 고정된 랜덤 4x4 테이블 정의
    int table[4][4] = {
        {3, 7, 11, 2},
        {14, 6, 9, 1},
        {12, 0, 5, 8},
        {10, 13, 4, 15}
    };

    // 테이블 출력
    printf("4x4 고정 랜덤 테이블:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%2d ", table[i][j]);
        }
        printf("\n");
    }

    // 4비트 2진수 입력 받기
    int binaryInput;
    printf("4비트 2진수를 입력하세요 (예: 1101): ");
    scanf("%d", &binaryInput);

    // 입력된 2진수를 행과 열로 변환
    int row = (binaryInput >> 2) & 0x3; // 첫 2비트
    int col = binaryInput & 0x3;        // 뒤 2비트

    // 변환된 행과 열 출력
    printf("행: %d, 열: %d\n", row, col);
    printf("선택된 값: %d\n", table[row][col]);

    return 0;
}