#include "types.h"

int strncmp(char *s1, char *s2, int n)
{
  int	i;

  i = 0;
  while ((s1[i] != 0 && (s2[i] != 0)) && (i < n))
  {
      if ((s1[i] != s2[i]) && (i < n))
        break;
    i += 1;
  }
  if ((s1[i] > s2[i]) && (i < n))
    return (1);
  else if ((s1[i] < s2[i]) && (i < n))
    return (-1);
  else
    return (0);

}
