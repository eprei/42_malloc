#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*ptr;

	ptr = 0;
	if (s1 != 0 && set != 0)
	{
		start = 0;
		end = ft_strlen(s1);
		while (s1[start] && ft_strchr(set, s1[start]))
			start++;
		while (s1[end - 1] && ft_strchr(set, s1[end - 1]) && end > start)
			end--;
		ptr = (char *)malloc(sizeof(char) * (end - start + 1));
		if (ptr)
			ft_strlcpy(ptr, &s1[start], end - start + 1);
	}
	return (ptr);
}
/*
int	main()
{
	char * s = ft_strtrim("   xxxtripouille", " x");
	printf("%s\n", s);
	return (0);
}

	char * s = ft_strtrim("   xxxtripouille", " x");
 check(!strcmp(s, "tripouille"));
 mcheck(s, strlen("tripouille") + 1); free(s); showLeaks();

	s = ft_strtrim("tripouille   xxx", " x");
 check(!strcmp(s, "tripouille"));
 mcheck(s, strlen("tripouille") + 1); free(s); showLeaks();

	s = ft_strtrim("   xxxtripouille   xxx", " x");
 check(!strcmp(s, "tripouille"));
 mcheck(s, strlen("tripouille") + 1); free(s); showLeaks();
	
	s = ft_strtrim("   xxx   xxx", " x");
 check(!strcmp(s, ""));
 mcheck(s, 1); free(s); showLeaks();

	s = ft_strtrim("", "123");
 check(!strcmp(s, ""));
 mcheck(s, 1); free(s); showLeaks();

	s = ft_strtrim("123", "");
 check(!strcmp(s, "123"));
/ mcheck(s, 4); free(s); showLeaks();

	s = ft_strtrim("", "");
 check(!strcmp(s, ""));
 mcheck(s, 1); free(s); showLeaks();
	
	s = ft_strtrim("abcdba", "acb");
check(!strcmp(s, "d"));*/