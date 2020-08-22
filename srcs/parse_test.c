/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/17 14:48:07 by thgermai          #+#    #+#             */
/*   Updated: 2020/08/22 16:53:48 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		check_closed(char *str)
{
	int			i;
	int			in_quote;
	int			in_dquote;

	in_quote = 0;
	in_dquote = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (i > 0 && str[i - 1] == -1)
				;
			else if (str[i] == '\'' && !in_dquote)
				in_quote == 1 ? in_quote-- : in_quote++;
			else if (str[i] == '"' && !in_quote)
				in_dquote == 1 ? in_dquote-- : in_dquote++;
		}
	}
	if (in_dquote || in_quote)
	{
		ft_printf_e("Minishell: error: arguments not closed\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void		parse_backslash(char *str) // replace backslashes by -1
{
	int			i;
	int			in_quote;
	int			in_dquote;

	in_quote = 0;
	in_dquote = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '"' &&
			(i == 0 || (i > 0 && str[i - 1] != -1)) && !in_quote)
			in_dquote == 1 ? in_dquote-- : in_dquote++;
		else if (str[i] == '\'' &&
			(i == 0 || (i > 0 && str[i - 1] != -1)) && !in_dquote)
			in_quote == 1 ? in_quote-- : in_quote++;
		if (str[i] == '\\' && (i == 0 || (i > 0 && str[i - 1] != -1)))
		{
			if (!in_quote && !in_dquote)
				str[i] = -1;
			else if (in_dquote && str[i + 1] && ft_strchr("$\\\"", str[i + 1]))  // ICI ft_strchr -> pas le simple quote
				str[i] = -1;
		}
	}
}

static void		parse_quotes(char *str) // replace quotes and dquotes by -2
{
	int			i;
	int			in_quote;
	int			in_dquote;

	in_quote = 0;
	in_dquote = 0;
	i = -1;
	while (str[++i])
	{
		if (i > 0 && str[i - 1] == -1)
			;
		else if (str[i] == '\'' && !in_dquote)
		{
			in_quote == 1 ? in_quote -- : in_quote++;
			str[i] = -2;
		}
		else if (str[i] == '"' && !in_quote)
		{
			in_dquote == 1 ? in_dquote-- : in_dquote++;
			str[i] = -2;
		}
	}
}

static char		*get_var_name(char *str) // parse the name from the arguments
{
	int			i;
	char		*var_name;

	i = -1;
	if (str[0] == '?')
		return (ft_strdup("?="));
	if (!ft_isalpha(str[0]) && str[0] != '_') // check if str[0] is aplha (error if digit)
		return (ft_strdup("00"));
	while (str[++i])
		if (!ft_isalnum(str[i]) && str[i] != '_')
			break ;
	if (!(var_name = malloc(sizeof(char) * (i + 2))))
		return (NULL);
	var_name[i + 1] = '\0';
	var_name[i] = '=';
	while (i-- > 0)
		var_name[i] = str[i];
	return (var_name);
}

static char		*fill_var1(char *str, int index, t_list **env)
{
	char		*var_name;
	char		*var_value;
	char		*new_str;
	char		*after_var;

	after_var = NULL;
	if (!(var_name = get_var_name(&str[index + 1])))
		return (str);
	if (!ft_strncmp(var_name, "00", 3))
		var_value = ft_strdup("");
	else if (var_name[0] == '?' && ft_strlen(var_name) == 2)
		var_value = ft_itoa(g_exit_status);
	else if (!ft_strncmp(var_name, "_=", 3))
		var_value = ft_strdup(g_last);
	else if (find_value(var_name, env, 1))
		var_value = ft_strdup(find_value(var_name, env, 1) + ft_strlen(var_name));
	else
		var_value = ft_strdup("");
	if (ft_strlen(str) - ft_strlen(var_name) + ft_strlen(var_value) == 0)
		new_str = ft_strdup("");
	else if (!(new_str = malloc(sizeof(char) * (ft_strlen(str) - ft_strlen(var_name) + ft_strlen(var_value)))))
		return (NULL);
	ft_strlcpy(new_str, str, index + 1);
	ft_strlcpy(new_str + ft_strlen(new_str), var_value, ft_strlen(var_value) + 1);
	if (str[index + ft_strlen(var_name)])
		after_var = ft_strdup(&str[index + ft_strlen(var_name)]);  // ICI a malloquer sinon erreur pour echo \"\\$TEST\|\"$1\\$444
	else
		after_var = ft_strdup("");
	ft_strlcpy(new_str + ft_strlen(new_str), after_var, ft_strlen(after_var) + 1);
	free(var_name);
	free(var_value);
	free(str);
	free(after_var);
	return (new_str);
}

static char		*get_var_value(char *var_name, int close, t_list **env) // fill_var2
{
	if (!close)
		return (ft_strdup("\0"));
	else if (var_name[0] == '?' && ft_strlen(var_name) == 2)
		return (ft_itoa(g_exit_status));
	else if (find_value(var_name, env, 1))
		return (ft_strdup(find_value(var_name, env, 1) + ft_strlen(var_name)));
	else
		return (ft_strdup("\0"));
}

static char		*fill_var2(char *str, int index, t_list **env)
{
	char		*var_name;
	char		*var_value;
	char		*new_str;
	char		*after_var;
	int			close;

	close = 0;
	var_name = get_var_name(&str[index + 2]);
	if (str[index + ft_strlen(var_name) + 1] == '}')
		close = 1;
	if (!close)
	{
		free(var_name);
		free(str);
		ft_printf_e("Minishell: unexpected EOF while looking for matching }\n");
		return (NULL);
	}
	var_value = get_var_value(var_name, close, env);
	if (!(new_str = malloc(sizeof(char) * (ft_strlen(str) - ft_strlen(var_name) + ft_strlen(var_value)))))
		return (NULL);
	ft_strlcpy(new_str, str, index + 1);
	ft_strlcpy(new_str + ft_strlen(new_str), var_value, ft_strlen(var_value) + 1);
	after_var = str + index + ft_strlen(var_name) + 2;
	ft_strlcpy(new_str + ft_strlen(new_str), after_var, ft_strlen(after_var) + 1);
	free(var_name);
	free(var_value);
	free(str);
	return (new_str);
}

static char		*fill_tilde(char *str, int index)
{
	char		*new_str;
	char		*after_var;

	if (!(new_str = malloc(sizeof(char) * (ft_strlen(str) - 1 + ft_strlen(g_home)))))
		return (NULL);
	ft_strlcpy(new_str, str, index + 1);
	ft_strlcpy(new_str + ft_strlen(new_str), g_home, ft_strlen(g_home) + 1);
	after_var = ft_strdup(str + index + 1);
	ft_strlcpy(new_str + ft_strlen(new_str), after_var, ft_strlen(after_var) + 1);
	free(str);
	free(after_var);
	return (new_str);
}

static char		*parse_var(char *str, t_list **env)
{
	int			i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$' && !is_valide(str, i, 0) && (i == 0 ||
			(i > 0 && str[i - 1] != -1)) && (ft_isalnum(str[i + 1]) ||
			str[i + 1] == '_' || str[i + 1] == '{' || str[i + 1] == '?'))
		{
			if (str[i + 1] == '{')
			{
				if (!(str = fill_var2(str, i, env)))
					return (NULL);
			}
			else
				str = fill_var1(str, i, env);
			if (!is_valide(str, i, 1) && !ft_strlen(str))
			{
				free(str);
				return (ft_strdup("\x80\xf5"));
			}
			i = -1;
		}
		else if (str[i] == '~' && !is_valide(str, i, 1))
			str = fill_tilde(str, i);
	}
	return (str);
}

static char		*replace_marks(char *str)
{
	int			i;
	int			j;
	char		*new_str;

	i = -1;
	j = 0;
	while (str[++i])
		if (str[i] > 0)
			j++;
	if (!j)
	{
		free(str);
		return (ft_strdup(""));
	}
	if (!(new_str = malloc(sizeof(char) * (j + 1))))
		return (NULL);
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] < 0)
		 	;
		else
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

static char		*parse_arg(char *str, t_list **env) // will be used to parse a str | need to make a func to cut args before parsing them
{
	if (check_closed(str))
	{
		free(str);
		return (NULL);
	}
	if (!(str = parse_var(str, env)))
		return (NULL);
	if (ft_strncmp(str, "\x80\xf5", ft_strlen(str)))
	{
		parse_quotes(str);
		str = replace_marks(str);
	}
	return (str);
}

static int		get_n_args(char *str)
{
	int				count;
	int				i;

	count = 0;
	i = -1;
	while (str[++i] == ' ')
		;
	while (str[++i])
	{
		if (str[i] == ' ' && str[i + 1] && str[i + 1]
			!= ' ' && !is_valide(str, i, 1))
			count++;
	}
	return (count);
}

static void		replace_g_last(char **last, char *last_arg)
{
	free(*last);
	*last = ft_strdup(last_arg);
}

char			**parse(char *str, t_list **env) // leaks here with the array (because it change length)
{
	char		**tab;
	int			i;
	int			n;
	int			n_args;

	n = 0;
	if (!str || !ft_strlen(str))
		return (NULL);
	parse_backslash(str);
	n_args = get_n_args(str) + 1;
	if (!(tab = malloc(sizeof(char *) * (n_args + 1))))
		return (NULL);
	while (n < n_args)
	{
		i = -1;
		while (*str == ' ')
			str++;
		while (str[++i])
			if (str[i] == ' ' && !is_valide(str, i, 1))
				break ;
		if (!(tab[n++] = parse_arg(ft_substr(str, 0, i), env)))
		{
			clean_array(tab);
			return (NULL);
		}
		if (!ft_strncmp(tab[n - 1], "\x80\xf5", 3))
		{
			free(tab[n - 1]);
			n--;
			n_args--;
		}
		str = str + i;
	}
	tab[n] = NULL;
	replace_g_last(&g_last, tab[n - 1]);
	return (tab);
}
