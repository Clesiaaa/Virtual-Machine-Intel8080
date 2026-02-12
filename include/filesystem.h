#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

// Virtual filesystem for 8080 emulator with persistent storage
#define MAX_FILES 100
#define MAX_DIRS 50
#define MAX_PATH 256
#define MAX_NAME 64
#define MAX_CONTENT 65536
#define FS_STORAGE_FILE ".emulator_fs.dat"

// File types
typedef enum {
    TYPE_FILE,
    TYPE_DIR
} FileType;

// File structure
typedef struct {
    char name[MAX_NAME];
    FileType type;
    char path[MAX_PATH];
    char content[MAX_CONTENT];
    uint32_t size;
    int parent_dir;
} File;

// Filesystem
typedef struct {
    File files[MAX_FILES];
    int file_count;
    char current_path[MAX_PATH];
    int current_dir;
} FileSystem;

// Filesystem functions
void init_filesystem(FileSystem *fs);
int create_file(FileSystem *fs, const char *name);
int create_dir(FileSystem *fs, const char *name);
int change_dir(FileSystem *fs, const char *name);
void list_files(FileSystem *fs);
int delete_file(FileSystem *fs, const char *name);
int write_file(FileSystem *fs, const char *name, const char *content);
int read_file(FileSystem *fs, const char *name, char *buffer);
char* get_prompt(FileSystem *fs);
int file_exists(FileSystem *fs, const char *name);
int get_file_index(FileSystem *fs, const char *name);

// Persistent storage
int save_filesystem(FileSystem *fs);
int load_filesystem(FileSystem *fs);

// Import from host
int import_file(FileSystem *fs, const char *host_path, const char *name);

// Text editor
int edit_file(FileSystem *fs, const char *filename);

#endif
