#include <stdio.h>

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
    {86, 87, 91, 92, 93, 94, 95, 96, 97, 98}};

// 역 테이블
int reverseTable[100] = {0};

// 역 테이블 초기화
void initializeReverseTable()
{
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      int value = mixedTable[i][j];
      reverseTable[value] = i * 10 + j;
    }
  }
}

// 인코딩 함수
int encodeNumber(int number)
{
  int row = number / 10; // 행 번호 (0부터 시작)
  int col = number % 10; // 열 번호 (0부터 시작)
  return mixedTable[row][col];
}

// 디코딩 함수
int decodeNumber(int encodedNumber)
{
  return reverseTable[encodedNumber];
}

int main()
{
  int number, encoded, decoded;

  // 역 테이블 초기화
  initializeReverseTable();

  // 사용자로부터 숫자 입력
  printf("0~99 사이의 숫자를 입력하세요: ");
  scanf("%d", &number);

  if (number < 0 || number >= 100)
  {
    printf("유효하지 않은 입력입니다. 0~99 사이의 숫자를 입력하세요.\n");
    return 1;
  }

  // 인코딩
  encoded = encodeNumber(number);
  printf("인코딩된 숫자: %d\n", encoded);

  // 디코딩
  decoded = decodeNumber(encoded);
  printf("디코딩된 숫자: %d\n", decoded);

  return 0;
}