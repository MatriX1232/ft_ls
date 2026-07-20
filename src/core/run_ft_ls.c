#include "ft_ls.h"

#include <stdio.h>

int	run_ft_ls(const t_config *config)
{
	int	status;
	int	i;
	int	multi;
	int	print_header;

	status = 0;
	i = 0;
	multi = (config->target_count > 1);
	print_header = (multi || config->options.recursive);
	while (i < config->target_count)
	{
		if (list_path(config->targets[i], &config->options, print_header) != 0)
			status = 1;
		if (multi && i + 1 < config->target_count)
			printf("\n");
		i++;
	}
	return (status);
}
