#include "types.h"

char	*strcat(char *dest, char *src)
{
  int	i;
  int	j;

  i = strlen(dest);
  j = 0;
  while (*(src + j))
    {
      *(dest + i + j) = *(src + j);
      j += 1;
    }
  *(dest + i + j) = 0x0;
  return (dest);
}
