#ifndef FT_LS_H
# define FT_LS_H

# include <stdbool.h>

typedef struct s_options
{
	bool	show_all;
	bool	reverse;
	bool	recursive;
	bool	time_sort;
	bool	long_format;
}t_options;

typedef struct s_config
{
	t_options	options;
	char		**targets;
	int			target_count;
}t_config;

int	parse_args(int argc, char **argv, t_config *config);
void	print_help(const char *program_name);
void	free_config(t_config *config);
int	run_ft_ls(const t_config *config);
int	list_path(const char *path, const t_options *opts, bool print_header);

#endif
