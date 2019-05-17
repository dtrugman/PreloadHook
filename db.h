#pragma once

int db_init(size_t db_size, const char *db_path);
void db_fini();
int db_find(const char *entry);
