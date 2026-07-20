#include "ft_ls.h"

#include <stdio.h>

void	print_help(const char *program_name)
{
	printf("Usage: %s [options] [file ...]\n", program_name);
	printf("Options:\n");
	printf("  -a  include directory entries whose names begin with a dot\n");
	printf("  -l  use a long listing format (template placeholder)\n");
	printf("  -r  reverse order while sorting\n");
	printf("  -R  list subdirectories recursively (template placeholder)\n");
	printf("  -t  sort by modification time (template placeholder)\n");
	printf("  --help  display this help and exit\n");
}
