#include "types.h"

char	*strncpy(char *dest, char *src, int n)
{
  int	i;

  i = 0;
  while ((i < n) && (*(src + i)))
    {
      *(dest + i) = *(src + i);
      i += 1;
    }
  while (i < n)
    {
      *(dest + i) = 0x0;
      i += 1;
    }
  return (dest);
}
