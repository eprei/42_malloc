#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		i;
	size_t		k;
	size_t		needle_len;
	char		*ptr;

	i = 0;
	ptr = (char *)haystack;
	needle_len = ft_strlen(needle);
	if (needle_len == 0 || haystack == needle)
		return (ptr);
	while (ptr[i] && i < len)
	{
		k = 0;
		while (ptr[i + k] != '\0' && needle[k] != '\0'
			&& ptr[i + k] == needle[k] && i + k < len)
			k++;
		if (k == needle_len)
			return (ptr + i);
		i++;
	}
	return (0);
}
/*
int	main()
{
	char haystack[30] = "aaabcabcd";
	char needle[10] = "aabc";
	char * empty = (char*)"";

	printf("%s\n", ft_strnstr(haystack, needle, 0));
	return (0);
}

	check(ft_strnstr(haystack, needle, 0) == 0); showLeaks();
	check(ft_strnstr(haystack, needle, -1) == haystack + 1); showLeaks();
	check(ft_strnstr(haystack, "a", -1) == haystack); showLeaks();
	check(ft_strnstr(haystack, "c", -1) == haystack + 4); showLeaks();
	check(ft_strnstr(empty, "", -1) == empty); showLeaks();
	check(ft_strnstr(empty, "", 0) == empty); showLeaks();
	check(ft_strnstr(empty, "coucou", -1) == 0); showLeaks();
	check(ft_strnstr(haystack, "aaabc", 5) == haystack); showLeaks();
	check(ft_strnstr(empty, "12345", 5) == 0); showLeaks();
	check(ft_strnstr(haystack, "abcd", 9) == haystack + 5); showLeaks();
	check(ft_strnstr(haystack, "cd", 8) == NULL); showLeaks();
	check(ft_strnstr(haystack, "a", 1) == haystack); showLeaks();
	check(ft_strnstr("1", "a", 1) == NULL); showLeaks();
	check(ft_strnstr("22", "b", 2) == NULL); showLeaks();
*/