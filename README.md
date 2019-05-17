# Preload Hook

An implementation of a glibc user-space hook using a preloaded library.

After compilation, a `proxy.so` file is created.

This shared library reads a list of file names from a predefined "db" file and hides them from `readdir` calls.

## Building

Just run `make`

## Using

You can preload the `proxy.so` file in two ways:

### 1. Explicitly preload the library using `LD_PRELOAD`

Just run any process you want with the prefix `LD_PRELOAD=<path-to-proxy.so>`, for example:

    LD_PRELOAD=./proxy.so ls /tmp

### 2. Use it globally using `/etc/ld.so.preload`

Just appen the absolute path of `proxy.so` to the `/etc/ld.so.preload` file using:

    sudo bash -c "echo <path-to-proxy.so> >> /etc/ld.so.preload"

OR use the Makefile command:

    make hook

## Configuration

By default, the implementation uses the file `/tmp/ccr0J61R.txt.swp` as the db.

If you want to edit the file, just run `make db`.

If you want to use a different path, change the const value inside `proxy.c`.

The "db" format is pure simple: every line is a file name to hide.

## Goals

This project is merely an example of the concept. Here are two examples of things you could do with it:

### 1. Hiding processes from `ps`

`ps` uses `opendir` and `readdir` to enumerate all running processes.
If we were to add the value `1000` to our "db" file and preload `proxy.so`, `ps` would never show us processes with pid `1000`.

### 2. Hiding files in `/tmp/`

`ls` uses `opendir` and `readdir` to enumerate all the files in a directory.
If we were to add the name `im_a_secret_file.txt` to our "db" file and preload

