#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int decrypt(int xor_key, unsigned char *out, size_t *outlen, unsigned char *in, size_t inlen)
{
  unsigned char encrypted_key[17] = {
    0x51, 0x7d, 0x7c, 0x75, 0x60, 0x73, 0x66, 0x67,
    0x7e, 0x73, 0x66, 0x7b, 0x7d, 0x7c, 0x61, 0x33, 0x00
  };
  size_t key_len = 16;
  int result;

  // Decrypt the key by XORing with xor_key
  for (size_t i = 0; i < key_len; i++) {
    encrypted_key[i] ^= xor_key;
  }

  // Compare decrypted key with "Congratulations!"
  if (memcmp(encrypted_key, "Congratulations!", key_len) == 0) {
    result = system("/bin/sh");  // Grant shell access
  } else {
    result = puts("\nInvalid Password");
  }

  return result;
}

void test(int input1, int input2)
{
  int xor_key;
  unsigned char *output;
  size_t *output_len;
  unsigned char *input;
  size_t input_len;

  xor_key = input2 - input1;
  switch(xor_key) {
  default:
    xor_key = rand();
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 1:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 2:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 3:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 4:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 5:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 6:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 7:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 8:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 9:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 16:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 17:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 18:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 19:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 20:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  case 21:
    decrypt(xor_key, output, output_len, input, input_len);
    break;
  }
  return;
}

int main(void)
{
  unsigned int seed;

  seed = time(NULL);
  srand(seed);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  scanf();
  test();
  return 0;
}
