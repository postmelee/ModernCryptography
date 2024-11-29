#include <stdio.h>
#include <string.h>
#include <time.h>

// ���� ������ �����ϴ� ����ü ����
typedef struct {
    char character;     // ����
    int numericValue;   // ���� ��
    char binary[9];     // 8��Ʈ ������ ���ڿ�
    char xorResult[9];  // XOR ��� ������ ���ڿ�
    char shifted[9];    // Shift Row ���
    char first4Bits[5]; // ù 4��Ʈ
    char second4Bits[5];// ��° 4��Ʈ
    int first4Decimal;  // ù 4��Ʈ�� 10���� ��
    int second4Decimal; // ��° 4��Ʈ�� 10���� ��
} CharInfo;
// ���� -> ���� ��ȯ �Լ�
int charToNumeric(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 0; // 'a'�� 0���� ����
    }
    else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 0; // 'A'�� 0���� ����
    }
    else {
        return -1; // ��ȿ���� ���� ���� ó��
    }
}

// ������ 8��Ʈ ������ ���ڿ��� ��ȯ�ϴ� �Լ�
void toBinary8Bit(int num, char* binary) {
    for (int i = 7; i >= 0; i--) {
        binary[i] = (num % 2) + '0'; // �������� �̿��Ͽ� ������ ���
        num /= 2;
    }
    binary[8] = '\0'; // ���ڿ� ���� NULL �߰�
}

// �� ���� 8��Ʈ ������ ���ڿ��� XOR �����ϴ� �Լ�
void xorBinaryStrings(char* binary1, char* binary2, char* result) {
    for (int i = 0; i < 8; i++) {
        // �� ��Ʈ�� ������ ��ȯ�Ͽ� XOR ���� ��, �ٽ� ���ڷ� ��ȯ
        result[i] = ((binary1[i] - '0') ^ (binary2[i] - '0')) + '0';
    }
    result[8] = '\0'; // ���ڿ� ���� NULL �߰�
}

// 8��Ʈ �������� ��ȯ ����Ʈ�ϴ� �Լ�
void shiftRow(char* binary, int shiftAmount, int direction) {
    char temp[9]; // �ӽ� ���� �迭
    int len = 8;  // 8��Ʈ ���� ����

    if (direction == 0) { // ���� ��ȯ ����Ʈ
        for (int i = 0; i < len; i++) {
            temp[i] = binary[(i + shiftAmount) % len];
        }
    }
    else if (direction == 1) { // ������ ��ȯ ����Ʈ
        for (int i = 0; i < len; i++) {
            temp[i] = binary[(i - shiftAmount + len) % len];
        }
    }

    temp[len] = '\0'; // NULL ���� �߰�
    strcpy_s(binary, 9, temp); // ������ ���ڿ� ����
}

// 8��Ʈ �������� 4��Ʈ�� ������ �Լ�
// 8��Ʈ �������� 4��Ʈ�� ������ �Լ�
void splitTo4Bits(char* binary, char* first4, char* second4) {
    strncpy_s(first4, 5, binary, 4);  // ù 4��Ʈ ���� (5�� �迭 ũ��)
    strncpy_s(second4, 5, binary + 4, 4); // ���� 4��Ʈ ���� (5�� �迭 ũ��)
}

int binaryToDecimal(char* binary) {
    int decimal = 0;
    int length = strlen(binary);
    for (int i = 0; i < length; i++) {
        decimal = decimal * 2 + (binary[i] - '0');
    }
    return decimal;
}



// ���� �ð��� ������ �ÿ� ���� ���� ��ȯ�ϴ� �Լ�
int getCurrentTime() {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    return local->tm_hour + local->tm_min;
}

int main() {
    char input[100];
    printf("���� �Է��ϼ���: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // ���� ���� ����

    // ���� �ð��� �ÿ� �� �� ���
    int timeSum = getCurrentTime();
    char timeBinary[9];
    toBinary8Bit(timeSum, timeBinary);

    printf("���� �ð� (��+��) ��: %d, 8��Ʈ ������: %s\n", timeSum, timeBinary);

    // �Է¹��� ���ڵ��� ������ ����ü �迭
    CharInfo charInfos[100];
    int charCount = 0;

    // ���� �� ���� ó��
    for (int i = 0; i < strlen(input); i++) {
        int numeric = charToNumeric(input[i]);
        if (numeric == -1) {
            printf("���� '%c'�� ��ȯ�� �� �����ϴ�.\n", input[i]);
            continue;
        }

        CharInfo info;
        info.character = input[i];
        info.numericValue = numeric;

        // ���ڸ� �������� ��ȯ
        toBinary8Bit(numeric, info.binary);

        // ������ XOR ��� ���
        xorBinaryStrings(info.binary, timeBinary, info.xorResult);

        // XOR ����� Shift Row ����
        strcpy_s(info.shifted, 9, info.xorResult);
        shiftRow(info.shifted, 2, 0); // �������� 2��Ʈ ����Ʈ

        // Shift Row ����� 4��Ʈ�� �и�
        splitTo4Bits(info.shifted, info.first4Bits, info.second4Bits);

        // 4��Ʈ�� 10������ ��ȯ
        info.first4Decimal = binaryToDecimal(info.first4Bits);
        info.second4Decimal = binaryToDecimal(info.second4Bits);

        // ����ü �迭�� ����
        charInfos[charCount++] = info;
    }

    // ��� ���
    for (int i = 0; i < charCount; i++) {
        printf("����: %c, ����: %d, 8��Ʈ ������: %s, XOR ���: %s, Shift Row ���: %s, ù 4��Ʈ: %s (%d), ��° 4��Ʈ: %s (%d)\n",
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
