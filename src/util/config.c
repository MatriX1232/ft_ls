#include "ft_ls.h"

#include <stdlib.h>

void	free_config(t_config *config)
{
	int	i;

	if (config->targets == NULL)
		return ;
	i = 0;
	while (i < config->target_count)
	{
		free(config->targets[i]);
		i++;
	}
	free(config->targets);
	config->targets = NULL;
	config->target_count = 0;
}
