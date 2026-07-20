#include "ft_ls.h"

#include "libft.h"

#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

typedef struct s_entry
{
	char		*name;
	char		*full_path;
	struct stat	st;
}t_entry;

typedef struct s_entries
{
	t_entry	*items;
	size_t	count;
	size_t	capacity;
}t_entries;

static const t_options	*g_sort_opts;

static char	*join_path(const char *path, const char *name)
{
	char	*full;
	size_t	path_len;
	size_t	name_len;
	int		need_slash;

	path_len = ft_strlen(path);
	name_len = ft_strlen(name);
	need_slash = (path_len > 0 && path[path_len - 1] != '/');
	full = malloc(path_len + (size_t)need_slash + name_len + 1);
	if (full == NULL)
		return (NULL);
	memcpy(full, path, path_len);
	if (need_slash)
		full[path_len++] = '/';
	memcpy(full + path_len, name, name_len);
	full[path_len + name_len] = '\0';
	return (full);
}

static int	cmp_entries(const void *a, const void *b)
{
	const t_entry	*left;
	const t_entry	*right;

	left = (const t_entry *)a;
	right = (const t_entry *)b;
	if (g_sort_opts != NULL && g_sort_opts->time_sort)
	{
		if (left->st.st_mtime > right->st.st_mtime)
			return (-1);
		if (left->st.st_mtime < right->st.st_mtime)
			return (1);
	}
	return (strcmp(left->name, right->name));
}

static int	entries_push(t_entries *entries, const char *path, const char *entry_name)
{
	t_entry	*new_items;
	t_entry	*slot;

	if (entries->count == entries->capacity)
	{
		entries->capacity = entries->capacity * 2 + 8;
		new_items = realloc(entries->items, entries->capacity * sizeof(t_entry));
		if (new_items == NULL)
			return (1);
		entries->items = new_items;
	}
	slot = &entries->items[entries->count];
	slot->name = ft_strdup(entry_name);
	if (slot->name == NULL)
		return (1);
	slot->full_path = join_path(path, entry_name);
	if (slot->full_path == NULL)
		return (free(slot->name), 1);
	if (lstat(slot->full_path, &slot->st) != 0)
	{
		fprintf(stderr, "ft_ls: %s: %s\n", slot->full_path, strerror(errno));
		free(slot->name);
		free(slot->full_path);
		return (0);
	}
	entries->count++;
	return (0);
}

static void	entries_free(t_entries *entries)
{
	size_t	i;

	i = 0;
	while (i < entries->count)
	{
		free(entries->items[i].name);
		free(entries->items[i].full_path);
		i++;
	}
	free(entries->items);
}

static void	mode_to_string(mode_t mode, char out[11])
{
	out[0] = '-';
	if (S_ISDIR(mode))
		out[0] = 'd';
	else if (S_ISLNK(mode))
		out[0] = 'l';
	else if (S_ISCHR(mode))
		out[0] = 'c';
	else if (S_ISBLK(mode))
		out[0] = 'b';
	else if (S_ISFIFO(mode))
		out[0] = 'p';
	else if (S_ISSOCK(mode))
		out[0] = 's';
	out[1] = (mode & S_IRUSR) ? 'r' : '-';
	out[2] = (mode & S_IWUSR) ? 'w' : '-';
	out[3] = (mode & S_IXUSR) ? 'x' : '-';
	out[4] = (mode & S_IRGRP) ? 'r' : '-';
	out[5] = (mode & S_IWGRP) ? 'w' : '-';
	out[6] = (mode & S_IXGRP) ? 'x' : '-';
	out[7] = (mode & S_IROTH) ? 'r' : '-';
	out[8] = (mode & S_IWOTH) ? 'w' : '-';
	out[9] = (mode & S_IXOTH) ? 'x' : '-';
	out[10] = '\0';
}

static void	format_time(time_t stamp, char out[32])
{
	struct tm	*tm_info;

	tm_info = localtime(&stamp);
	if (tm_info == NULL)
	{
		ft_strlcpy(out, "??? ?? ??:??", 32);
		return ;
	}
	strftime(out, 32, "%b %e %H:%M", tm_info);
}

static void	print_long_entry(const t_entry *entry)
{
	char			mode[11];
	char			timebuf[32];
	struct passwd	*pw;
	struct group	*gr;

	mode_to_string(entry->st.st_mode, mode);
	format_time(entry->st.st_mtime, timebuf);
	pw = getpwuid(entry->st.st_uid);
	gr = getgrgid(entry->st.st_gid);
	printf("%s %2llu %s %s %8lld %s %s\n",
		mode,
		(unsigned long long)entry->st.st_nlink,
		pw ? pw->pw_name : "?",
		gr ? gr->gr_name : "?",
		(long long)entry->st.st_size,
		timebuf,
		entry->name);
}

static int	list_directory(const char *path, const t_options *opts, bool print_header)
{
	DIR			*dir;
	struct dirent	*entry;
	t_entries	entries;
	long long	total;
	ssize_t		i;
	ssize_t		end;
	ssize_t		step;
	int			status;

	dir = opendir(path);
	if (dir == NULL)
		return (fprintf(stderr, "ft_ls: %s: %s\n", path, strerror(errno)), 1);
	entries = (t_entries){0};
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (opts->show_all || entry->d_name[0] != '.')
			if (entries_push(&entries, path, entry->d_name) != 0)
				return (closedir(dir), entries_free(&entries), perror("ft_ls: memory"), 1);
		entry = readdir(dir);
	}
	closedir(dir);
	g_sort_opts = opts;
	qsort(entries.items, entries.count, sizeof(t_entry), cmp_entries);
	if (print_header)
		printf("%s:\n", path);
	if (opts->long_format)
	{
		total = 0;
		i = 0;
		while ((size_t)i < entries.count)
			total += entries.items[i++].st.st_blocks;
		printf("total %lld\n", total / 2);
	}
	if (opts->reverse)
	{
		i = (ssize_t)entries.count - 1;
		end = -1;
		step = -1;
	}
	else
	{
		i = 0;
		end = (ssize_t)entries.count;
		step = 1;
	}
	while (i != end)
	{
		if (opts->long_format)
			print_long_entry(&entries.items[i]);
		else
			printf("%s\n", entries.items[i].name);
		i += step;
	}
	status = 0;
	if (opts->recursive)
	{
		if (opts->reverse)
		{
			i = (ssize_t)entries.count - 1;
			end = -1;
			step = -1;
		}
		else
		{
			i = 0;
			end = (ssize_t)entries.count;
			step = 1;
		}
		while (i != end)
		{
			if (S_ISDIR(entries.items[i].st.st_mode)
				&& strcmp(entries.items[i].name, ".") != 0
				&& strcmp(entries.items[i].name, "..") != 0)
			{
				printf("\n");
				if (list_directory(entries.items[i].full_path, opts, true) != 0)
					status = 1;
			}
			i += step;
		}
	}
	entries_free(&entries);
	return (status);
}

int	list_path(const char *path, const t_options *opts, bool print_header)
{
	struct stat	st;
	t_entry		entry;

	if (lstat(path, &st) != 0)
		return (fprintf(stderr, "ft_ls: %s: %s\n", path, strerror(errno)), 1);
	if (S_ISDIR(st.st_mode))
		return (list_directory(path, opts, print_header));
	if (opts->long_format)
	{
		entry.name = (char *)path;
		entry.st = st;
		print_long_entry(&entry);
	}
	else
		printf("%s\n", path);
	return (0);
}
