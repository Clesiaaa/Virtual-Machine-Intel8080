#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "../../include/filesystem.h"

// ===================================================================
// Virtual Filesystem Implementation with Persistent Storage
// ===================================================================

// Initialize default files
void create_default_files(FileSystem *fs) {
    // Hello World program
    const char *hello_asm =
        "; Hello World for Intel 8080\n"
        "MVI A, 72    ; 'H'\n"
        "OUT 1\n"
        "MVI A, 69    ; 'E'\n"
        "OUT 1\n"
        "MVI A, 76    ; 'L'\n"
        "OUT 1\n"
        "MVI A, 76    ; 'L'\n"
        "OUT 1\n"
        "MVI A, 79    ; 'O'\n"
        "OUT 1\n"
        "MVI A, 32    ; ' '\n"
        "OUT 1\n"
        "MVI A, 87    ; 'W'\n"
        "OUT 1\n"
        "MVI A, 79    ; 'O'\n"
        "OUT 1\n"
        "MVI A, 82    ; 'R'\n"
        "OUT 1\n"
        "MVI A, 76    ; 'L'\n"
        "OUT 1\n"
        "MVI A, 68    ; 'D'\n"
        "OUT 1\n"
        "MVI A, 10    ; '\\n'\n"
        "OUT 1\n"
        "HLT\n";

    // Create examples directory
    int examples_idx = create_dir(fs, "examples");
    if (examples_idx >= 0) {
        change_dir(fs, "examples");

        create_file(fs, "hello.asm");
        write_file(fs, "hello.asm", hello_asm);

        change_dir(fs, "..");
    }
}

void init_filesystem(FileSystem *fs) {
    // Try to load existing filesystem
    if (load_filesystem(fs) == 0) {
        printf("Filesystem loaded from disk\n");
        return;
    }

    // Create new filesystem
    fs->file_count = 0;
    strcpy(fs->current_path, "root");
    fs->current_dir = -1;

    // Create root directory
    strcpy(fs->files[0].name, "root");
    fs->files[0].type = TYPE_DIR;
    strcpy(fs->files[0].path, "root");
    fs->files[0].parent_dir = -1;
    fs->files[0].size = 0;
    fs->file_count = 1;

    // Create default example files
    create_default_files(fs);

    // Save the initial filesystem
    save_filesystem(fs);

    printf("Filesystem initialized\n");
}

// Save filesystem to disk
int save_filesystem(FileSystem *fs) {
    FILE *f = fopen(FS_STORAGE_FILE, "wb");
    if (!f) {
        printf("Error: Cannot save filesystem\n");
        return -1;
    }

    fwrite(fs, sizeof(FileSystem), 1, f);
    fclose(f);
    return 0;
}

// Load filesystem from disk
int load_filesystem(FileSystem *fs) {
    FILE *f = fopen(FS_STORAGE_FILE, "rb");
    if (!f) {
        return -1;
    }

    fread(fs, sizeof(FileSystem), 1, f);
    fclose(f);
    return 0;
}

// Import file from host system
int import_file(FileSystem *fs, const char *host_path, const char *name) {
    FILE *f = fopen(host_path, "r");
    if (!f) {
        printf("Error: Cannot open host file '%s'\n", host_path);
        return -1;
    }

    char buffer[MAX_CONTENT];
    size_t size = fread(buffer, 1, MAX_CONTENT - 1, f);
    buffer[size] = '\0';
    fclose(f);

    int idx = create_file(fs, name);
    if (idx < 0) {
        return -1;
    }

    write_file(fs, name, buffer);
    save_filesystem(fs);

    printf("Imported '%s' from host\n", name);
    return 0;
}

int get_file_index(FileSystem *fs, const char *name) {
    for (int i = 0; i < fs->file_count; i++) {
        if (fs->files[i].parent_dir == fs->current_dir &&
            strcmp(fs->files[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int file_exists(FileSystem *fs, const char *name) {
    return get_file_index(fs, name) != -1;
}

int create_file(FileSystem *fs, const char *name) {
    if (fs->file_count >= MAX_FILES) {
        printf("Error: Maximum files reached\n");
        return -1;
    }

    if (file_exists(fs, name)) {
        printf("Error: '%s' already exists\n", name);
        return -1;
    }

    File *file = &fs->files[fs->file_count];
    strcpy(file->name, name);
    file->type = TYPE_FILE;
    sprintf(file->path, "%s/%s", fs->current_path, name);
    file->parent_dir = fs->current_dir;
    file->size = 0;
    file->content[0] = '\0';

    fs->file_count++;
    save_filesystem(fs);
    printf("File '%s' created\n", name);
    return fs->file_count - 1;
}

int create_dir(FileSystem *fs, const char *name) {
    if (fs->file_count >= MAX_FILES) {
        printf("Error: Maximum directories reached\n");
        return -1;
    }

    if (file_exists(fs, name)) {
        printf("Error: '%s' already exists\n", name);
        return -1;
    }

    File *file = &fs->files[fs->file_count];
    strcpy(file->name, name);
    file->type = TYPE_DIR;
    sprintf(file->path, "%s/%s", fs->current_path, name);
    file->parent_dir = fs->current_dir;
    file->size = 0;

    fs->file_count++;
    save_filesystem(fs);
    printf("Directory '%s' created\n", name);
    return fs->file_count - 1;
}

int change_dir(FileSystem *fs, const char *name) {
    if (strcmp(name, "..") == 0) {
        if (fs->current_dir == -1) {
            printf("Already in root\n");
            return -1;
        }

        int parent = fs->files[fs->current_dir].parent_dir;
        fs->current_dir = parent;

        if (parent == -1) {
            strcpy(fs->current_path, "root");
        } else {
            strcpy(fs->current_path, fs->files[parent].path);
        }

        return 0;
    }

    int idx = get_file_index(fs, name);

    if (idx == -1) {
        printf("Directory '%s' not found\n", name);
        return -1;
    }

    if (fs->files[idx].type != TYPE_DIR) {
        printf("'%s' is not a directory\n", name);
        return -1;
    }

    fs->current_dir = idx;
    strcpy(fs->current_path, fs->files[idx].path);

    return 0;
}

void list_files(FileSystem *fs) {
    int count = 0;

    printf("\n");
    printf("Directory: %s\n", fs->current_path);
    printf("================================================\n");

    for (int i = 0; i < fs->file_count; i++) {
        if (fs->files[i].parent_dir == fs->current_dir) {
            if (fs->files[i].type == TYPE_DIR) {
                printf("  [DIR]  %-30s\n", fs->files[i].name);
            } else {
                printf("  [FILE] %-30s %6d B\n",
                       fs->files[i].name, fs->files[i].size);
            }
            count++;
        }
    }

    if (count == 0) {
        printf("  (empty)\n");
    }

    printf("================================================\n");
    printf("  %d item(s)\n\n", count);
}

int delete_file(FileSystem *fs, const char *name) {
    int idx = get_file_index(fs, name);

    if (idx == -1) {
        printf("'%s' not found\n", name);
        return -1;
    }

    if (fs->files[idx].type == TYPE_DIR) {
        for (int i = 0; i < fs->file_count; i++) {
            if (fs->files[i].parent_dir == idx) {
                printf("Directory '%s' is not empty\n", name);
                return -1;
            }
        }
    }

    for (int i = idx; i < fs->file_count - 1; i++) {
        fs->files[i] = fs->files[i + 1];
    }

    fs->file_count--;
    save_filesystem(fs);
    printf("'%s' deleted\n", name);
    return 0;
}

int write_file(FileSystem *fs, const char *name, const char *content) {
    int idx = get_file_index(fs, name);

    if (idx == -1) {
        printf("File '%s' not found\n", name);
        return -1;
    }

    if (fs->files[idx].type != TYPE_FILE) {
        printf("'%s' is not a file\n", name);
        return -1;
    }

    strcpy(fs->files[idx].content, content);
    fs->files[idx].size = strlen(content);
    save_filesystem(fs);

    return 0;
}

int read_file(FileSystem *fs, const char *name, char *buffer) {
    int idx = get_file_index(fs, name);

    if (idx == -1) {
        printf("File '%s' not found\n", name);
        return -1;
    }

    if (fs->files[idx].type != TYPE_FILE) {
        printf("'%s' is not a file\n", name);
        return -1;
    }

    strcpy(buffer, fs->files[idx].content);
    return fs->files[idx].size;
}

char* get_prompt(FileSystem *fs) {
    static char prompt[MAX_PATH + 10];
    sprintf(prompt, "%s> ", fs->current_path);
    return prompt;
}

// Simple text editor (vim-like)
int edit_file(FileSystem *fs, const char *filename) {
    int idx = get_file_index(fs, filename);

    if (idx == -1) {
        printf("File '%s' not found\n", filename);
        return -1;
    }

    if (fs->files[idx].type != TYPE_FILE) {
        printf("'%s' is not a file\n", filename);
        return -1;
    }

    char buffer[MAX_CONTENT];
    strcpy(buffer, fs->files[idx].content);

    system("clear");
    printf("=== EDITOR: %s ===\n", filename);
    printf("Press ESC then :w to save, :q to quit, :wq to save and quit\n");
    printf("--------------------------------------------------\n\n");

    // Simple line-based editor
    char lines[1000][256];
    int line_count = 0;

    // Parse existing content into lines
    char *token = strtok(buffer, "\n");
    while (token != NULL && line_count < 1000) {
        strcpy(lines[line_count++], token);
        token = strtok(NULL, "\n");
    }

    // Display current content
    for (int i = 0; i < line_count; i++) {
        printf("%3d | %s\n", i + 1, lines[i]);
    }

    printf("\n--------------------------------------------------\n");
    printf("Commands:\n");
    printf("  :w    - Save\n");
    printf("  :q    - Quit\n");
    printf("  :wq   - Save and quit\n");
    printf("  a     - Append new line\n");
    printf("  d NUM - Delete line NUM\n");
    printf("\n> ");

    char cmd[256];
    int running = 1;

    while (running) {
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            break;
        }

        cmd[strcspn(cmd, "\n")] = '\0';

        if (strcmp(cmd, ":w") == 0) {
            // Save
            buffer[0] = '\0';
            for (int i = 0; i < line_count; i++) {
                strcat(buffer, lines[i]);
                if (i < line_count - 1) {
                    strcat(buffer, "\n");
                }
            }
            write_file(fs, filename, buffer);
            printf("File saved\n");
        }
        else if (strcmp(cmd, ":q") == 0) {
            running = 0;
        }
        else if (strcmp(cmd, ":wq") == 0) {
            buffer[0] = '\0';
            for (int i = 0; i < line_count; i++) {
                strcat(buffer, lines[i]);
                if (i < line_count - 1) {
                    strcat(buffer, "\n");
                }
            }
            write_file(fs, filename, buffer);
            printf("File saved\n");
            running = 0;
        }
        else if (cmd[0] == 'a') {
            printf("Enter line (empty to finish):\n");
            char new_line[256];
            if (fgets(new_line, sizeof(new_line), stdin)) {
                new_line[strcspn(new_line, "\n")] = '\0';
                if (strlen(new_line) > 0 && line_count < 1000) {
                    strcpy(lines[line_count++], new_line);
                    printf("Line added\n");
                }
            }
        }
        else if (cmd[0] == 'd') {
            int line_num = atoi(cmd + 2);
            if (line_num > 0 && line_num <= line_count) {
                for (int i = line_num - 1; i < line_count - 1; i++) {
                    strcpy(lines[i], lines[i + 1]);
                }
                line_count--;
                printf("Line %d deleted\n", line_num);
            }
        }

        if (running) {
            printf("\n> ");
        }
    }

    return 0;
}
