#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	src_len;
	size_t	dst_len;

	j = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	i = dst_len;
	if (size == 0)
		return (src_len);
	if (size < dst_len)
		return (src_len + size);
	else
	{
		while (src[j] && (dst_len + j) < size)
			dst[i++] = src[j++];
		if ((dst_len + j) == size && dst_len < size)
			dst[--i] = '\0';
		else
			dst[i] = '\0';
		return (src_len + dst_len);
	}
}

/*
int	main()
{
	char str1[27] = "Hello World";
	char str2[100] = " whats up 123456789";
	char ptr1[27] = "Hello world";
	char ptr2[100] = " whats up 123456789";
	char dest[30]; memset(dest, 0, 30);
	char * src = (char *)"AAAAAAAAA";
	dest[0] = 'B';

	char *str = "the cake is a lie !\0I'm hidden lol\r\n";
	char buff1[0xF00] = "there is no stars in the sky";
	char buff2[0xF00] = "there is no stars in the sky";
	size_t max = 0;

	ft_strlcat(dest, src, 1);
	ft_strlcat(buff2, str, max);
	strlcat(buff2, str, max);	
	printf("the result is:\t%s\n", dest);
	strlcat(str2, str1, 0);
	ft_strlcat(str2, str1, 0);

	return(0);
}*/