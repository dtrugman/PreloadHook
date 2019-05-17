#include <stdlib.h>
#include <errno.h>

#include "log.h"

#define LENGTHOF(x) ((sizeof(x))/(sizeof(x[0])))

static struct {
    size_t size;
    size_t entries;
    char ** records;
} g_db;

static int db_load(const char *db_path)
{
    FILE* fp = fopen(db_path, "r");
    if (!fp) {
        ERR("fopen [%s]", db_path);
        return -errno;
    }

    int read_err = -ERANGE;
    for (size_t i = 0; i < g_db.size; ++i) {
        errno = 0;
        char *line = NULL;
        size_t line_len = 0;
        ssize_t len = getline(&line, &line_len, fp);
        if (len == -1) {
            read_err = errno; // If we gracefully reached EOF, should be 0
            free(line);
            break;
        }

        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        g_db.records[g_db.entries++] = line;
    }

    int close_err = 0;
    if (fclose(fp) == EOF) {
        close_err = errno;
    }

    errno = (read_err != 0) ? read_err : close_err;
    return (errno == 0) ? 0 : -1;
}

int db_init(size_t db_size, const char *db_path)
{
    g_db.size = db_size;
    g_db.entries = 0;
    g_db.records = (char **)malloc(g_db.size * sizeof(g_db.records[0]));
    if (!g_db.records) {
        ERR("malloc");
        return -1;
    }

    return db_load(db_path);
}

void db_fini()
{
    for (size_t i = 0; i < g_db.entries; ++i) {
        free(g_db.records[i]);
    }

    if (g_db.records) {
        free(g_db.records);
    }
}

int db_find(const char *entry)
{
    for (size_t i = 0; i < g_db.entries; ++i) {
        if (strcmp(g_db.records[i], entry) == 0) {
            return i;
        }
    }

    return -1;
}
