#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned long long int	len_s1;
	unsigned long long int	len_s2;
	char					*ptr;
	unsigned long long int	i;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	ptr = malloc(len_s1 + len_s2 + (1 * (sizeof(char))));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[i - len_s1] != '\0')
	{
		ptr[i] = s2[i - len_s1];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
