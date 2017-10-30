#include "types.h"

void	underflow(unsigned int n)
{
  if (n > 9)
      underflow(n / 10);
  putchar((n % 10) + '0');
}

void	putnbr(int n)
{
  if (n == -2147483648)
    {
      putchar('-');
      return (underflow(n));
    }
  if (n < 0)
    {
      putchar('-');
      n = n * -1;
    }
  if (n > 9)
      putnbr(n / 10);
  putchar((n % 10) + '0');
}
