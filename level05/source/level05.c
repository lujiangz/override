#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void) {

    char buffer[100];  
    unsigned int index = 0;
    size_t len;

    fgets(buffer, 100, stdin);
    index = 0;

    while (1) {
        len = strlen(buffer);

        if (index >= len) {

            printf(buffer); 
            
            exit(0);
        }

        // Büyük Harf -> Küçük Harf Dönüşümü
        // '@' (64) ve '[' (91) ASCII değerleri arasını kontrol eder (A-Z arası).
        if (buffer[index] > '@' && buffer[index] < '[') {
            // 0x20 (32) ile XOR işlemi, ASCII'de büyük harfi küçüğe çevirir.
            // Örnek: 'A' (0x41) ^ 0x20 = 'a' (0x61)
            buffer[index] = buffer[index] ^ 0x20;
        }
        index = index + 1;
    }
}