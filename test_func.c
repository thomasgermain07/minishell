#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libft/libft.h"

static char		*get_key(char *str)
{
	char 		*key;
	int			i;

	i = -1;
	while (str[++i])
		if (str[i] == '=')
			break ;
	return (ft_substr(str, 0, i + 1));
}

int main()
{
	printf("key = %s\n", get_key("PATH=testici"));
	return (0);
}
