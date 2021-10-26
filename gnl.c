#include <stdlib.h>
#include <unistd.h>

int ft_strlen(char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (s[i])
        i++;
    return (i);
}

char    *find_line_break(char *s)
{
    int i;

    i = 0;
    if (!s)
        return (NULL);
    while (s[i])
    {
        if (s[i] == '\n')
            return (&s[i]);
        i++;
    } 
    return (NULL);
}

char    *ft_join(char *s, char *v)
{
    int i = 0;
    char *new;

    if (!s || !v)
        return (NULL);
    new = malloc(sizeof(char) * (ft_strlen(s) + ft_strlen(v) + 1));
    if (!new)
        return (NULL);
    while (*s)
        new[i++] = *s++;
    while (*v)
        new[i++] = *v++;
    new[i] = '\0';
    return (new);
}

char    *ft_substr(char *s, int start, size_t len)
{
    int i = 0;
    char *new;

    if (!s)
        return (NULL);
    new = malloc(sizeof(char) * (len + 1));
    if (!new)
        return (NULL);
    while (len)
    {
        new[i++] = s[start++];
        len--;
    }
    new[i] = '\0';
    return (new);
}

char    *purge_line_from_tmp(char *tmp, char *line)
{
    char *new;

    if (!find_line_break(tmp))
    {
        free(tmp);
        return (NULL);
    }
    new = ft_substr(tmp, ft_strlen(line), ft_strlen(tmp) - ft_strlen(line));
    free(tmp);
    return (new);
}

char    *tmp_to_line(char *tmp)
{
    char *new;

    if (find_line_break(tmp))
        new = ft_substr(tmp, 0, ft_strlen(tmp) - ft_strlen(find_line_break(tmp) + 1));
    else
        new = ft_substr(tmp, 0, ft_strlen(tmp));
    return (new);
}

char    *update_tmp(char *tmp, char *buf)
{
    char *new;

    if (!tmp)
    {
        tmp = malloc(1);
        tmp[0] = '\0';
    }
    new = ft_join(tmp, buf);
    free(tmp);
    return (new);
}

char    *read_to_tmp(int fd, char *tmp)
{
    char buf[BUFFER_SIZE + 1];
    int bytes;

    bytes = 1;
    while (!find_line_break(tmp) && bytes > 0)
    {
        bytes = read(fd, buf, BUFFER_SIZE);
        if ((bytes == 0 && !tmp) || bytes == -1)
            return (NULL);
        buf[bytes] = '\0';
        tmp = update_tmp(tmp, buf);
    }
    return (tmp);
}

char *gnl(int fd)
{
    static char *tmp;
    char *line;

    line = NULL;
    if (BUFFER_SIZE <= 0 || fd < 0)
        return (NULL);
    tmp = read_to_tmp(fd, tmp);
    line = tmp_to_line(tmp);
    tmp = purge_line_from_tmp(tmp, line);
    return (line);
}

#include <stdio.h>
#include <fcntl.h>

int		main(int argc, char **argv)
{
	int		fd;
	// int		ret;
	char	*line;

	fd = 0;
	// ret = 1;
	if (argc > 1)
		fd = open(argv[1], O_RDONLY);
	while ((line = gnl(fd)))
    {
		printf("line = |%s|\n", line);
    }
    free(line);
    // fd = 0;
    // free(get_next_line(fd));
	return (0);
}