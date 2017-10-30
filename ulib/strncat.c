#include "types.h"

char	*strncat(char *dest, char *src, int n)
{
  int	dlen;
  int	i;

  i = 0;
  dlen = strlen(dest);
  while ((i < n) && (*(src + i)))
    {
      *(dest + dlen + i) = *(src + i);
      i += 1;
    }
  *(dest + dlen + i) = 0x0;
  return (dest);
}
