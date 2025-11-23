#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// Decompiler çıktısında DAT_08048ad0 olarak görünen, "%u" format stringi.
const char *SCANF_FMT = "%u"; 

// Buffer'ı temizleyen yardımcı fonksiyon
void clear_stdin(void) {
    int ch;
    do {
        ch = getchar();
        if ((char)ch == '\n') {
            return;
        }
    } while ((char)ch != -1); // EOF kontrolü
}

// Kullanıcıdan unsigned sayı alan fonksiyon
unsigned int get_unum(void) {
    unsigned int num = 0;
    fflush(stdout);
    scanf(SCANF_FMT, &num);
    clear_stdin();
    return num;
}

// Diziden sayı okuma fonksiyonu (Zaafiyet potansiyeli: Index kontrolü yok)
int read_number(int *data_storage) {
    unsigned int index;
    
    printf(" Index: ");
    index = get_unum();
    
    // Burada sınır kontrolü (bounds check) yok.
    // data_storage dizisinin dışına çıkılabilir (Out-of-Bounds Read).
    printf(" Number at data[%u] is %u\n", index, data_storage[index]);
    return 0;
}

// Diziye sayı kaydetme fonksiyonu (Index kontrolü zayıf)
int store_number(int *data_storage) {
    unsigned int number;
    unsigned int index;
    
    printf(" Number: ");
    number = get_unum();
    
    printf(" Index: ");
    index = get_unum();
    
    // Kısıtlama kontrolü:
    // 1. Index 3'ün katı olamaz.
    // 2. Sayının en yüksek baytı (MSB) 0xb7 olamaz.
    if ((index % 3 == 0) || ((number >> 24) == 0xb7)) {
        puts(" *** ERROR! ***");
        puts("    This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1; // Hata kodu
    } else {
        // Burada da üst sınır kontrolü yok (Out-of-Bounds Write).
        // Sadece 3'ün katı olmayan herhangi bir pozitif index'e yazabilirsin.
        data_storage[index] = number;
        return 0; // Başarılı
    }
}

int main(int argc, char **argv, char **envp) {
    // env ve argv temizleyen döngüler
    // CTF sorularında genellikle exploit sırasında environment variable kullanılmasın diye yapılır.
    if (argv) {
        for (char **ptr = argv; *ptr != 0; ptr++) {
             memset(*ptr, 0, strlen(*ptr));
        }
    }
    if (envp) {
        for (char **ptr = envp; *ptr != 0; ptr++) {
             memset(*ptr, 0, strlen(*ptr));
        }
    }

    int data_storage[100]; // local_1bc [100]
    char command_buf[20];  // local_28 [20]
    int result_status = 0; // local_2c

    // data_storage dizisini sıfırla
    memset(data_storage, 0, sizeof(data_storage));
    
    // command_buf dizisini sıfırla
    memset(command_buf, 0, sizeof(command_buf));

    puts(
      "----------------------------------------------------\n"
      "  Welcome to wil's crappy number stora ge service!    \n"
      "----------------------------------------------------\n"
      " Commands:                                          \n"
      "    store - store a number into the data storage    \n"
      "    read  - read a number from the data storage     \n"
      "    quit  - exit the program                        \n"
      "----------------------------------------------------\n"
      "   wil has reserved some storage :>                 \n"
      "----------------------------------------------------\n"
    );

    while (true) {
        printf("Input command: ");
        result_status = 1; // Varsayılan olarak hata var kabul et
        
        // Kullanıcıdan komut al
        fgets(command_buf, sizeof(command_buf), stdin);

        // Satır sonu karakterini (\n) temizle (Decompiler'daki strlen ve loop mantığı)
        size_t len = strlen(command_buf);
        if (len > 0 && command_buf[len - 1] == '\n') {
            command_buf[len - 1] = '\0';
        }

        // Komutları kontrol et
        if (strncmp(command_buf, "store", 5) == 0) {
            result_status = store_number(data_storage);
        }
        else if (strncmp(command_buf, "read", 4) == 0) {
            result_status = read_number(data_storage);
        }
        else if (strncmp(command_buf, "quit", 4) == 0) {
            return 0; // Programdan çık
        }
        
        // Komut sonucunu yazdır
        if (result_status == 0) {
            printf(" Completed %s command successfully\n", command_buf);
        } else {
            printf(" Failed to do %s command\n", command_buf);
        }

        // Buffer'ı tekrar sıfırla
        memset(command_buf, 0, sizeof(command_buf));
    }
}