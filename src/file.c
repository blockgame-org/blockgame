#include <blockgame/file.h>

#include <blockgame/log.h>
#include <blockgame/utility.h>

#include <stdio.h>

int
bg_read_file(char **out, char const *filename)
{
    FILE *fd = fopen (filename, "r");

    if (!fd || 0 != fseek(fd, 0, SEEK_END))
        goto READ_ERROR;

    long length = ftell(fd);

    if (-1 == length)
        goto READ_ERROR;

    if (0 != fseek(fd, 0, SEEK_SET))
        goto READ_ERROR;

    char *content = bg_calloc(length + 1, sizeof(char));
    size_t read = fread(content, sizeof(char), length, fd);

    if (0 != ferror(fd) || read != length)
        goto READ_ERROR;

    fclose(fd);
    *out = content;
    return 0;

READ_ERROR:
    // TODO: Get more error information
    bg_error("Failed to read file '%s'", filename);
    return -1;
}