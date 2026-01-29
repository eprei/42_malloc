#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char* ptr1 = dst;
	unsigned char* ptr2 = (unsigned char*)src;

	if (src == NULL && dst == NULL){
		return NULL;
	}

	while (n > 0){
		*ptr1 = *ptr2;
		ptr1++;
		ptr2++;
		n--;
	}

	return dst;
}
