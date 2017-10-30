#include "types.h"

char *strcpy(char *dest, char *src)
{
  int i;

  i = 0;
  while (*(src + i))
  {
    *(dest + i) = *(src + i);
    i += 1;
  }
  *(dest + i) = 0x0;
  return (dest);
}
