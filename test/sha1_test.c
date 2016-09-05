#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  srand(time(NULL));

  uint32_t r = 0;
  r = (uint32_t) rand();

  printf("%02X\n", r);

  r = 0;

  for (int i = 0; i < 200; i++) {
    r = (uint32_t) rand();
    printf("%08X\n", r);
  }

  return 0;

}