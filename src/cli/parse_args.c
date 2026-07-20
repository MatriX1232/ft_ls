#include "ft_ls.h"

#include "libft.h"

#include <stdio.h>
#include <stdlib.h>

static int	parse_flag_group(const char *arg, t_options *opts)
{
	int	i;

	i = 1;
	while (arg[i] != '\0')
	{
		if (arg[i] == 'a')
			opts->show_all = true;
		else if (arg[i] == 'r')
			opts->reverse = true;
		else if (arg[i] == 'R')
			opts->recursive = true;
		else if (arg[i] == 't')
			opts->time_sort = true;
		else if (arg[i] == 'l')
			opts->long_format = true;
		else
		{
			fprintf(stderr, "ft_ls: illegal option -- %c\n", arg[i]);
			fprintf(stderr, "Try '--help' for more information.\n");
			return (2);
		}
		i++;
	}
	return (0);
}

static int	add_target(t_config *config, const char *value, int *index)
{
	config->targets[*index] = ft_strdup(value);
	if (config->targets[*index] == NULL)
	{
		perror("ft_ls: ft_strdup");
		return (1);
	}
	(*index)++;
	config->target_count = *index;
	return (0);
}

int	parse_args(int argc, char **argv, t_config *config)
{
	int	i;
	int	insert;
	int	status;

	ft_memset(config, 0, sizeof(*config));
	config->targets = ft_calloc((size_t)argc, sizeof(char *));
	if (config->targets == NULL)
		return (perror("ft_ls: ft_calloc"), 1);
	i = 1;
	insert = 0;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "--help", 7) == 0 && argv[i][6] == '\0')
			return (print_help(argv[0]), free_config(config), 0);
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			status = parse_flag_group(argv[i], &config->options);
			if (status != 0)
				return (free_config(config), status);
		}
		else if (add_target(config, argv[i], &insert) != 0)
			return (free_config(config), 1);
		i++;
	}
	if (insert == 0 && add_target(config, ".", &insert) != 0)
		return (free_config(config), 1);
	return (0);
}
