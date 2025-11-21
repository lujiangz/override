#include <stdio.h>
#include <stdlib.h>

int main(void)

{
  int input_buffer[4];
  
  puts("***********************************");
  puts("* \t     -Level00 -\t\t  *");
  puts("***********************************");
  printf("Password:");

  scanf("%d", input_buffer);

  if (input_buffer[0] != 0x149c) {
    puts("\nInvalid Password!");
  }
  else {
    puts("\nAuthenticated!");
    system("/bin/sh");
  }

  return input_buffer[0] != 0x149c;
}