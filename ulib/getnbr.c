#include "types.h"

int getnbr(char *str)
{
  int	i;
  int	n;
  int	neg;

  i = 0;
  n = 0;
  neg = 1;
  while ((str[i] == '+') || (str[i] == '-'))
    {
      if (str[i] == '-')
	  neg = neg * -1;
      i += 1;
    }
  while ((str[i]) && ((str[i] >= 48) && (str[i] <= 57)))
    {
      n = (n * 10) + (str[i] - '0');
      i += 1;
    }
  return (n * neg);
}
