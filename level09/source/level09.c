#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Stack'teki ofsetlere göre düzenlenmiştir.
// strncpy(param_1, local_408, (long)*(int *)(param_1 + 0xb4)) gibi bir kullanım olduğu için
// (param_1 + 0xb4)
typedef struct {
    char message[140];   // 0x00 - 0x8C arası (140 byte)
    char username[40];   // 0x8C - 0xB4 arası (40 byte)
    int msg_len;         // 0xB4 ofsetinde (Bu değişken kritik!)
} context_t;

void handle_msg(void);
void set_username(context_t *ctx);
void set_msg(context_t *ctx);
void secret_backdoor(void);

int main(void) {
    puts("--------------------------------------------");
    puts("|    ~Welcome to l33t-m$n ~    v1337       |");
    puts("--------------------------------------------");
    
    handle_msg();
    
    return 0;
}

void handle_msg(void) {
    context_t ctx;

    memset(&ctx, 0, sizeof(context_t));

    // Mesaj uzunluğunu varsayılan olarak 140 (0x8c) ayarla
    ctx.msg_len = 0x8c; 

    set_username(&ctx);
    set_msg(&ctx);

    puts(">: Msg sent!");
}

void set_username(context_t *ctx) {
    char input_buffer[140];
    int i;

    memset(input_buffer, 0, sizeof(input_buffer));

    puts(">: Enter your username");
    printf(">>: ");
    
    // Kullanıcıdan 128 byte oku
    fgets(input_buffer, 128, stdin);

    // Username alanı 40 byte, ancak döngü 0x29 (41) kez dönüyor.
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (i = 0; i < 41 && input_buffer[i] != '\0'; i++) {
        ctx->username[i] = input_buffer[i];
    }

    printf(">: Welcome, %s\n", ctx->username);
}

void set_msg(context_t *ctx) {
    char msg_buffer[1024];

    memset(msg_buffer, 0, sizeof(msg_buffer));

    puts(">: Msg @Unix-Dude");
    printf(">>: ");

    // Kullanıcıdan 1024 byte oku
    fgets(msg_buffer, 1024, stdin);

    // Mesajı kopyala. Uzunluk 'ctx->msg_len'den alınıyor.
    // Eğer 'set_username' fonksiyonunda msg_len bozulduysa,
    // burada overflow olabilir.
    strncpy(ctx->message, msg_buffer, ctx->msg_len);
}

void secret_backdoor(void) {
    char cmd_buffer[128];

    fgets(cmd_buffer, 128, stdin);
    system(cmd_buffer);
}