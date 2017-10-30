#include "types.h"

void putchar(char c)
{
  write(1, &c, sizeof(char));
}
