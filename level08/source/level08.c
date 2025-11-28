#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void log_wrapper(FILE *log_file, char *message, char *filename);

int main(int argc, char **argv) {
    int out_fd;
    int ch;
    FILE *log_fp;
    FILE *src_fp;
    char dest_path[104];
    
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
    }

    log_fp = fopen("./backups/.log", "w");
    if (log_fp == NULL) {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }

    // İlk logu yaz
    log_wrapper(log_fp, "Starting back up: ", argv[1]);

    // Kaynak dosyayı okuma modunda aç
    src_fp = fopen(argv[1], "r");
    if (src_fp == NULL) {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }

    strncpy(dest_path, "./backups/", 11); // 0xb = 11
    strncat(dest_path, argv[1], 100 - strlen(dest_path) - 1);

    // Hedef dosyayı açma (Yazma, Oluşturma modları)
    // 0xc1 = O_WRONLY | O_CREAT | O_EXCL (Genelde)
    // 0x1b0 = 0660 (rw-rw----) izinleri
    out_fd = open(dest_path, O_WRONLY | O_CREAT | O_EXCL, 0660);
    if (out_fd < 0) {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(1);
    }

    while (1) {
        ch = fgetc(src_fp);
        if (ch == EOF) break; // -1 EOF'tur
        char byte_to_write = (char)ch;
        write(out_fd, &byte_to_write, 1);
    }

    log_wrapper(log_fp, "Finished back up ", argv[1]);

    fclose(src_fp);
    close(out_fd);

    return 0;
}

void log_wrapper(FILE *log_file, char *message, char *filename) {
    char log_buffer[264]; // local_118
    size_t newline_pos;
    size_t current_len;

    // Mesajı buffer'a kopyala ("Starting back up: " gibi)
    strcpy(log_buffer, message);

    current_len = strlen(log_buffer);

    // Eğer filename "%x %x" gibi şeyler içerirse stack sızdırabilir.
    snprintf(log_buffer + current_len, 264 - current_len, filename);

    // Yeni satır karakterini temizleme
    newline_pos = strcspn(log_buffer, "\n");
    log_buffer[newline_pos] = '\0';

    fprintf(log_file, "LOG: %s\n", log_buffer);

    return;
}