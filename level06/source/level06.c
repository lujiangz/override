#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

// Auth fonksiyonu: Giriş yapılan login string'ine göre bir serial hesaplar
// ve kullanıcının girdiği serial ile karşılaştırır.
int auth(char *login, unsigned int user_serial) {
    size_t len;
    unsigned int calculated_hash;
    int i;
    
    //Login stringindeki newline (\n) karakterini temizle
    len = strcspn(login, "\n");
    login[len] = '\0';
    
    //Uzunluk kontrolü
    len = strnlen(login, 32);
    if ((int)len < 6) {
        return 1; // Login en az 6 karakter olmalı
    }
    
    // Anti-Debugging Kontrolü (ptrace)
    // Eğer bir debugger (gdb, ltrace vb.) bu süreci izliyorsa -1 döner.
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
        puts("\x1b[32m.---------------------------.");
        puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1b[32m\'---------------------------\'");
        return 1;
    }
    
    // Serial Üretme Algoritması
    // Başlangıç değeri: Login'in 4. karakteri (index 3) ile 0x1337 XOR'lanır, 
    // üzerine 0x5eeded eklenir.
    calculated_hash = ((int)login[3] ^ 0x1337U) + 0x5eeded;
    
    for (i = 0; i < (int)len; i++) {
        // Kontrol karakteri kontrolü (ASCII 32'den küçükse hata ver)
        if (login[i] < ' ') {
            return 1;
        }
        
        // Hash hesaplama döngüsü
        // Mevcut karakter ile o anki hash XOR'lanır, 0x539'a (1337) göre modu alınır
        // ve toplama eklenir.
        unsigned int xor_val = (int)login[i] ^ calculated_hash;
        unsigned int mod_val = xor_val % 0x539;
        calculated_hash = calculated_hash + mod_val;
    }
    
    // Sonuç Kontrolü
    if (user_serial == calculated_hash) {
        return 0; // Başarılı
    } else {
        return 1; // Başarısız
    }
}

int main(void) {
    unsigned int serial_input;
    char login_buffer[32];
    int result;
    
    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    
    // Kullanıcıdan Login al
    printf("-> Enter Login: ");
    fgets(login_buffer, 32, stdin);
    
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    
    // Kullanıcıdan Serial al
    printf("-> Enter Serial: ");
    scanf("%u", &serial_input); 
    
    // Auth kontrolü
    result = auth(login_buffer, serial_input);
    
    if (result == 0) {
        puts("Authenticated!");
        system("/bin/sh"); // Hedef burası
    }
    
    return result != 0;
}