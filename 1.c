#include <stdint.h>
#include <stdio.h>

#define size 0xff

void example(uint8_t *str) {
  for (uint8_t i = 0; i < size; i++) {
    str[i] = i;
  }
}

void show1(uint8_t *str) {
  for (uint8_t i = 0; i < size; i++) {
    printf("%02x ", str[i]);
  }
  printf("\n");
}
void show(uint8_t *str) { show1(str); }

int main() {

  uint8_t str[size];
  example(str);
  show(str);
  int address = 0xF0;
  int len = 20;

  printf("size %d\n", sizeof(str));
  for (int i = 0; i < len; i++) {
    printf("%02x ", str[address + i]);
  }
  printf("\n");
  return 0;
}
