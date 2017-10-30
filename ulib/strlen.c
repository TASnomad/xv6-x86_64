#include "types.h"

int strlen(char *str)
{
  int i;

  i = 0;
  while (*(str + i))
    i += 1;
  return (i);
}
