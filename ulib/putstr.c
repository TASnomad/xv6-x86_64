#include "types.h"

void	putstr(char *str)
{
  while ((*str))
    {
      putchar(*str);
      str += 1;
    }
}
