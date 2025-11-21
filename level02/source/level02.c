#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char pass_from_file[41];      // local_a8  → 41 byte okunacak + null terminator
    char username[100];           // local_78
    char password[100];           // local_118  (ama stack'te daha yukarda!)
    int  bytes_read;
    FILE *fp;

    // Tüm bufferları sıfırla (anti-garbage)
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    memset(pass_from_file, 0, sizeof(pass_from_file));

    fp = fopen("/home/users/level03/.pass", "r");
    if (!fp) {
        fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
        exit(1);
    }

    // Tam 41 byte oku (0x29 = 41)
    bytes_read = fread(pass_from_file, 1, 41, fp);
    pass_from_file[strcspn(pass_from_file, "\n")] = '\0'; // newline'ı temizle

    if (bytes_read != 41) {
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        exit(1);
    }
    fclose(fp);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    printf("--[ Username: ");
    fgets(username, 100, stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("--[ Password: ");
    fgets(password, 100, stdin);
    password[strcspn(password, "\n")] = '\0';

    puts("*****************************************");

    // Şifre kontrolü: tam 41 byte karşılaştırılıyor
    if (strncmp(pass_from_file, password, 41) == 0) {
        printf("Greetings, %s!\n", username);
        system("/bin/sh");
        return 0;
    }

    printf(username);                   // !! nonnik bir kullanım :)
    puts(" does not have access!");
    exit(1);
}