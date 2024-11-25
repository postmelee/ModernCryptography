#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ��ȣȭ �Լ�
void encrypt(const char* plaintext, const char* key, char* ciphertext) {
    size_t textLen = strlen(plaintext);
    size_t keyLen = strlen(key);

    for (size_t i = 0, j = 0; i < textLen; i++) {
        char c = plaintext[i];

        if (isalpha(c)) {  // ���ĺ� ���ڸ� ��ȣȭ
            char base = islower(c) ? 'a' : 'A';
            ciphertext[i] = (c - base + (key[j % keyLen] - '0')) % 26 + base;
            j++; // Ű�� ���� ���ڷ� �̵�
        }
        else {
            ciphertext[i] = c; // ���ĺ��� �ƴϸ� �״�� ����
        }
    }
    ciphertext[textLen] = '\0'; // ���ڿ� ����
}

// ��ȣȭ �Լ�
void decrypt(const char* ciphertext, const char* key, char* plaintext) {
    size_t textLen = strlen(ciphertext);
    size_t keyLen = strlen(key);

    for (size_t i = 0, j = 0; i < textLen; i++) {
        char c = ciphertext[i];

        if (isalpha(c)) {  // ���ĺ� ���ڸ� ��ȣȭ
            char base = islower(c) ? 'a' : 'A';
            plaintext[i] = (c - base - (key[j % keyLen] - '0') + 26) % 26 + base;
            j++; // Ű�� ���� ���ڷ� �̵�
        }
        else {
            plaintext[i] = c; // ���ĺ��� �ƴϸ� �״�� ����
        }
    }
    plaintext[textLen] = '\0'; // ���ڿ� ����
}

// ���� �ð��� Ű�� ��ȯ
void generateKeyFromTime(char* key) {
    time_t currentTime;
    time(&currentTime);

    struct tm* localTime = localtime(&currentTime);

    // HHMM �������� Ű ����
    snprintf(key, 5, "%02d%02d", localTime->tm_hour, localTime->tm_min);
}

int main() {
    char plaintext[256];          // ����� �Է� ��
    char key[5];                  // �ð� ��� Ű (HHMM ����)
    char ciphertext[256];
    char decrypted[256];

    // �� �Է� �ޱ�
    printf("���� �Է��ϼ���: ");
    if (fgets(plaintext, sizeof(plaintext), stdin)) {
        plaintext[strcspn(plaintext, "\n")] = '\0'; // ���� ���� ����
    }

    // Ű ����
    generateKeyFromTime(key);
    printf("������ Ű: %s\n", key);

    // ��ȣȭ
    encrypt(plaintext, key, ciphertext);
    printf("��ȣȭ�� �ؽ�Ʈ: %s\n", ciphertext);

    // ��ȣȭ
    decrypt(ciphertext, key, decrypted);
    printf("��ȣȭ�� �ؽ�Ʈ: %s\n", decrypted);

    return 0;
}
