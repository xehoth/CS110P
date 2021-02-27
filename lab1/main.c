#include <stdio.h>
#include <time.h>
#include <stdint.h>
#define PRINT_SIZE(type) printf("Size of "#type": %d\n", (int)sizeof(type))

int main() {
  PRINT_SIZE(char);
  PRINT_SIZE(short);
  PRINT_SIZE(short int);
  PRINT_SIZE(int);
  PRINT_SIZE(long int);
  PRINT_SIZE(unsigned int);
  PRINT_SIZE(void *);
  PRINT_SIZE(size_t);
  PRINT_SIZE(float);
  PRINT_SIZE(double);
  PRINT_SIZE(int8_t);
  PRINT_SIZE(int16_t);
  PRINT_SIZE(int32_t);
  PRINT_SIZE(int64_t);
  PRINT_SIZE(time_t);
  PRINT_SIZE(clock_t);
  PRINT_SIZE(struct tm);
  PRINT_SIZE(NULL);

  return 0;
}