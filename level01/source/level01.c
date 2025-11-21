#include <stdio.h>
#include <stdbool.h>

// Global değişken (Ghidra'daki &a_user_name)
char global_username_buffer[256];

// Fonksiyon prototipleri
int verify_user_name(void);
int verify_user_pass(char *input_pass);

int main(void)
{
    int exit_code;
    int i;
    char *ptr_cleaner;
    char password_buffer[64];
    int auth_status;
  
    ptr_cleaner = password_buffer;
    for (i = 0x10; i != 0; i = i + -1) {
        ptr_cleaner[0] = '\0';
        ptr_cleaner[1] = '\0';
        ptr_cleaner[2] = '\0';
        ptr_cleaner[3] = '\0';
        ptr_cleaner = ptr_cleaner + 4;
    }
  
    auth_status = 0;
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    
    fgets(global_username_buffer, 0x100, stdin);
    
    auth_status = verify_user_name();
    
    if (auth_status == 0) {
        puts("Enter Password: ");
        
        //Zaafiyet burada. Buffer 64 byte ama 100 byte okunuyor (Buffer Overflow)
        fgets(password_buffer, 100, stdin);
        
        auth_status = verify_user_pass(password_buffer);
        
        // auth_status her durumda 0 veya 1 olur.
        if ((auth_status == 0) || (auth_status != 0)) {
            puts("nope, incorrect password...\n");
            exit_code = 1;
        }
        else {
            exit_code = 0;
        }
    }
    else {
        puts("nope, incorrect username...\n");
        exit_code = 1;
    }
    return exit_code;
}

// "dat_wil" kullanıcı adını kontrol eden fonksiyon
int verify_user_name(void)
{
    int length_counter;
    char *ptr_input;
    char *ptr_target;
    bool is_less_than;
    bool is_equal;
    
    puts("verifying username....\n");
    
    length_counter = 7; // "dat_wil" uzunluğu
    ptr_input = global_username_buffer;
    ptr_target = "dat_wil";
    
    do {
        if (length_counter == 0) break;
        length_counter = length_counter + -1;
        
        is_less_than = *ptr_input < *ptr_target;
        is_equal = *ptr_input == *ptr_target;
        
        ptr_input = ptr_input + 1;
        ptr_target = ptr_target + 1;
        
    } while (is_equal);
    
    return (int)(char)((!is_less_than && !is_equal) - is_less_than);
}

// "admin" şifresini kontrol eden fonksiyon
int verify_user_pass(char *input_pass)
{
    int length_counter;
    char *ptr_target;
    bool is_less_than;   // CF (Carry Flag) karşılığı
    bool is_equal;       // ZF (Zero Flag) karşılığı
  
    length_counter = 5; // "admin" uzunluğu
    ptr_target = "admin";
    
    do {
        if (length_counter == 0) break;
        length_counter = length_counter + -1;
        
        is_less_than = *input_pass < *ptr_target;
        is_equal = *input_pass == *ptr_target;
        
        input_pass = input_pass + 1;
        ptr_target = ptr_target + 1;
        
    } while (is_equal);
    
    return (int)(char)((!is_less_than && !is_equal) - is_less_than);
}