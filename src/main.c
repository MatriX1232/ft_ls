#include "ft_ls.h"

#include <stdio.h>

int	main(int argc, char **argv)
{
	t_config	config;
	int			status;

	status = parse_args(argc, argv, &config);
	if (status != 0)
		return (status);
	status = run_ft_ls(&config);
	free_config(&config);
	return (status);
}
