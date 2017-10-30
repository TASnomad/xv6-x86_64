#include "types.h"

int strcmp(char *s1, char *s2)
{
  int i;

  i = 0;
  while (*s1 == *s2)
  {
    if (!*s1)
      return (0);
      s1 += 1;
      s2 += 1;
  }
  i = *s1 - *s2;
  return (i < 0) ? -1 : 1;
}
