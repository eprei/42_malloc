#include "libft.h"

int	ft_printf(const char *str, ...)
{
	va_list		ap;
	int			count;

	count = 0;
	va_start(ap, str);
	while (*str){
		if (*str == '%')
		{
			str++;
			count += ft_switch(*str, ap);
		}
		else
			count += ft_putchar(*str);
		str++;
	}

	va_end(ap);
	return count;
}

int	ft_switch(char conversion, va_list ap)
{
	if (conversion == 'c')
		return ft_putchar(va_arg(ap, int));
	if (conversion == 's')
		return ft_putstr_printf(va_arg(ap, char *));
	if (conversion == 'p')
		return ft_str_putnbr_pointer(va_arg(ap, unsigned long));
	if (conversion == 'd' || conversion == 'i')
		return ft_putstr_itoa(ft_itoa(va_arg(ap, int)));
	if (conversion == 'u')
		return ft_u_itoa(va_arg(ap, unsigned int));
	if (conversion == 'x')
		return ft_putnbr_hex_min(va_arg(ap, unsigned int));
	if (conversion == 'X')
		return ft_putnbr_hex_maj(va_arg(ap, unsigned int));
	if (conversion == '%')
		return ft_putchar('%');
	return 0;
}

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return 1;
}

int	ft_putstr_printf(char *s)
{
	if (!s) {
		s = "(null)";
	}

	write(1, s, ft_strlen(s));

	return ft_strlen(s);
}


int	ft_putnbr_hex_min(unsigned int n)
{
	char	*str;
	int count = 0;
	str = "0123456789abcdef";

	if (n <= 15){
		write(1, &str[n], 1);
		count += 1;
	} else {
		count += ft_putnbr_hex_min(n / 16);
		count += ft_putnbr_hex_min(n % 16);
	}

	return count;
}

int	ft_putnbr_hex_maj(unsigned int n)
{
	char	*str;
	int count = 0;
	str = "0123456789ABCDEF";

	if (n <= 15){
		write(1, &str[n], 1);
		count += 1;
	} else {
		count += ft_putnbr_hex_maj(n / 16);
		count += ft_putnbr_hex_maj(n % 16);
	}

	return count;
}

int	ft_putnbr_pointer(unsigned long n)
{
	char	*str;
	int count = 0;
	str = "0123456789abcdef";

	if (n <= 15){
		write(1, &str[n], 1);
		count += 1;
	} else {
		count += ft_putnbr_pointer(n / 16);
		count += ft_putnbr_pointer(n % 16);
	}

	return count;
}

int	ft_str_putnbr_pointer(unsigned long n)
{
	int count = 0;

	count += ft_putstr_printf("0x");
	count += ft_putnbr_pointer(n);

	return count;
}

int	ft_putstr_itoa(char *str)
{
	int		count;
	char	*conversion;

	conversion = str;
	count = 0;
	while (*str)
	{
		write(1, str, 1);
		str++;
		count++;
	}
	free(conversion);
	return count;
}

int	ft_u_itoa(unsigned int n)
{
	char			*ptr;
	unsigned int	len_n;
	unsigned int	m;

	m = n;
	len_n = ft_u_intlen(m);
	ptr = (char *)malloc(sizeof(char) * (len_n + 1));
	if (ptr == NULL)
		return 0;
	if (len_n == 0 || m == 0)
		ptr[0] = '0';
	ptr[len_n] = '\0';

	while (m > 0){
		ptr[len_n - 1] = m % 10 + 48;
		m = m / 10;
		len_n--;
	}

	ft_putstr_printf(ptr);
	free(ptr);

	return ft_u_intlen(n);
}

int	ft_u_intlen(unsigned int c)
{
	int				i;
	unsigned int	x;

	x = c;
	i = 0;
	while (x > 9){
		x = x / 10;
		i++;
	}
	i++;

	return i;
}
