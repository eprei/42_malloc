#include "libft.h"

int	ft_intlen(int c)
{
	int			i;
	long int	x;

	x = c;
	i = 0;
	if (x < 0)
	{
		x = x * (-1);
		i++;
	}
	while (x > 9)
	{
		x = x / 10;
		i++;
	}
	i++;
	return (i);
}

char	*ft_itoa(int n)
{
	char		*ptr;
	int			len_n;
	long int	m;

	m = n;
	len_n = ft_intlen(m);
	ptr = (char *)malloc(sizeof(char) * (len_n + 1));
	if (ptr == NULL)
		return (NULL);
	if (len_n == 0 || m == 0)
		ptr[0] = '0';
	ptr[len_n] = '\0';
	if (m < 0)
	{
		ptr[0] = '-';
		m = m * (-1);
	}
	while (m > 0)
	{
		ptr[len_n - 1] = (m % 10) + 48;
		m = m / 10;
		len_n--;
	}
	return (ptr);
}
