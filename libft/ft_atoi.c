#include "libft.h"

static int	ft_is_maj(long long int c)
{
	if (c > 2147483647)
		return (1);
	return (0);
}

static int	ft_is_min(long long int c)
{
	if (c < -2147483648)
		return (1);
	return (0);
}

static int	ft_check(char c)
{
	if ((c == ' ') || (c == '\t') || (c == '\n')
		|| (c == '\v') || (c == '\f') || (c == '\r'))
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	long long int	result;
	int				sign;

	sign = 1;
	result = 0;
	while (ft_check(*str))
		str++;
	if (*str == '-' || *str == '+')
	{	
		if (*str++ == '-')
			sign = -1;
	}
	while (*str)
	{
		if (ft_is_maj(sign * result))
			return -1;
		if (ft_is_min(sign * result))
			return 0;
		if (*str >= '0' && *str <= '9')
			result = result * 10 + (*str - '0');
		else
			break ;
		str++;
	}
	return (sign * result);
}
