#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "include/cpu.h"
#include "include/ram.h"
#include "include/instructions.h"
#include "include/compile.h"
#include "include/filesystem.h"

int main() {
    system("clear");
    printf("booting.");
    sleep(1);

    cpu c;
    init_cpu(&c);
    printf(".");
    sleep(1);

    init_ram(&c);
    printf(".");
    sleep(1);

    // Initialize filesystem
    FileSystem fs;
    init_filesystem(&fs);
    printf(".\n");
    sleep(1);

    system("clear");
    printf("                                                                                \n");
    printf("▜▘   ▐     ▜ ▞▀▖▞▀▖▞▀▖▞▀▖\n");
    printf("▐ ▛▀▖▜▀ ▞▀▖▐ ▚▄▘▌▞▌▚▄▘▌▞▌\n");
    printf("▐ ▌ ▌▐ ▖▛▀ ▐ ▌ ▌▛ ▌▌ ▌▛ ▌\n");
    printf("▀▘▘ ▘ ▀ ▝▀▘ ▘▝▀ ▝▀ ▝▀ ▝▀ \n");
    printf("   ================================================================================\n");
    printf("   Kernel                                                                       \n");
    printf("                                                                                \n");
    printf(" -- version 2.0.0 --\n");
    printf(" -- Made by Robert Folga --\n");
    printf(" -- Persistent filesystem with vim-like editor --\n");
    printf("                                                                                \n");

    int running = 1;

    while (running) {
        char cmd[50];
        printf("%s", get_prompt(&fs));
        scanf("%49s", cmd);

        // ===================================================================
        // SYSTEM COMMANDS
        // ===================================================================

        if (strcmp(cmd, "clear") == 0) {
            system("clear");
        }
        else if (strcmp(cmd, "exit") == 0) {
            printf("Saving filesystem...\n");
            save_filesystem(&fs);
            printf("Goodbye!\n");
            running = 0;
        }

        // ===================================================================
        // FILESYSTEM COMMANDS
        // ===================================================================

        else if (strcmp(cmd, "ls") == 0) {
            list_files(&fs);
        }
        else if (strcmp(cmd, "cd") == 0) {
            char dirname[MAX_NAME];
            scanf("%s", dirname);
            change_dir(&fs, dirname);
        }
        else if (strcmp(cmd, "mkdir") == 0) {
            char dirname[MAX_NAME];
            printf("directory name: ");
            scanf("%s", dirname);
            create_dir(&fs, dirname);
        }
        else if (strcmp(cmd, "touch") == 0) {
            char filename[MAX_NAME];
            printf("file name: ");
            scanf("%s", filename);
            create_file(&fs, filename);
        }
        else if (strcmp(cmd, "del") == 0) {
            char name[MAX_NAME];
            printf("file/directory: ");
            scanf("%s", name);
            delete_file(&fs, name);
        }
        else if (strcmp(cmd, "cat") == 0) {
            char filename[MAX_NAME];
            printf("file: ");
            scanf("%s", filename);

            char buffer[MAX_CONTENT];
            if (read_file(&fs, filename, buffer) >= 0) {
                printf("\n--- Content of %s ---\n", filename);
                printf("%s\n", buffer);
                printf("--- End ---\n\n");
            }
        }
        else if (strcmp(cmd, "editor") == 0) {
            char filename[MAX_NAME];
            printf("file: ");
            scanf("%s", filename);
            edit_file(&fs, filename);
            system("clear");
        }
        else if (strcmp(cmd, "import") == 0) {
            char host_path[256];
            char fs_name[MAX_NAME];

            printf("host path: ");
            getchar();
            fgets(host_path, sizeof(host_path), stdin);
            host_path[strcspn(host_path, "\n")] = '\0';

            printf("name in filesystem: ");
            fgets(fs_name, sizeof(fs_name), stdin);
            fs_name[strcspn(fs_name, "\n")] = '\0';

            import_file(&fs, host_path, fs_name);
        }

        // ===================================================================
        // EMULATOR COMMANDS
        // ===================================================================

        else if (strcmp(cmd, "load") == 0) {
            char filename[MAX_NAME];
            printf("ROM file: ");
            scanf("%s", filename);

            char buffer[MAX_CONTENT];
            if (read_file(&fs, filename, buffer) >= 0) {
                int addr = 0;
                char *token = strtok(buffer, " \n\t");
                while (token != NULL) {
                    unsigned int byte;
                    sscanf(token, "%02X", &byte);
                    c.memory[addr++] = (uint8_t)byte;
                    token = strtok(NULL, " \n\t");
                }
                printf("ROM loaded: %d bytes\n", addr);
            }
        }
        else if (strcmp(cmd, "run") == 0) {
            run(&c);
            init_cpu(&c);
            init_ram(&c);
            printf("\n");
        }
        else if (strcmp(cmd, "exec") == 0) {
            char filename[MAX_NAME];
            printf("file: ");
            scanf("%s", filename);

            char buffer[MAX_CONTENT];
            if (read_file(&fs, filename, buffer) >= 0) {
                int addr = 0;
                char *token = strtok(buffer, " \n\t");
                while (token != NULL) {
                    unsigned int byte;
                    sscanf(token, "%02X", &byte);
                    c.memory[addr++] = (uint8_t)byte;
                    token = strtok(NULL, " \n\t");
                }
                printf("ROM loaded: %d bytes\n", addr);
                run(&c);
                init_cpu(&c);
                init_ram(&c);
                printf("\n");
            }
        }
        else if (strcmp(cmd, "compile") == 0) {
            char file_in[MAX_NAME];
            char file_out[MAX_NAME];

            printf("source file (.asm): ");
            scanf("%s", file_in);
            printf("output file (.hex): ");
            scanf("%s", file_out);

            char source[MAX_CONTENT];
            if (read_file(&fs, file_in, source) < 0) {
                printf("Error: source file not found\n");
                continue;
            }

            FILE *tmp = fopen("/tmp/temp_asm.asm", "w");
            fprintf(tmp, "%s", source);
            fclose(tmp);

            compile("/tmp/temp_asm.asm", "/tmp/temp_out.hex");

            FILE *out = fopen("/tmp/temp_out.hex", "r");
            char result[MAX_CONTENT];
            size_t size = fread(result, 1, MAX_CONTENT - 1, out);
            result[size] = '\0';
            fclose(out);

            int idx = create_file(&fs, file_out);
            if (idx >= 0) {
                write_file(&fs, file_out, result);
                printf("Compilation complete: %s\n", file_out);
            }
        }

        // ===================================================================
        // HELP
        // ===================================================================

        else if (strcmp(cmd, "help") == 0) {
            printf("\n================================================\n");
            printf("  Intel 8080 Emulator - Commands              \n");
            printf("================================================\n");
            printf("  SYSTEM                                       \n");
            printf("    clear   - Clear screen                     \n");
            printf("    help    - Show this help                   \n");
            printf("    exit    - Exit emulator                    \n");
            printf("                                               \n");
            printf("  FILES                                        \n");
            printf("    ls      - List files                       \n");
            printf("    cd      - Change directory                 \n");
            printf("    mkdir   - Create directory                 \n");
            printf("    touch   - Create file                      \n");
            printf("    del     - Delete file/directory            \n");
            printf("    cat     - Show file content                \n");
            printf("    editor  - Edit file (vim-like)             \n");
            printf("    import  - Import file from host            \n");
            printf("                                               \n");
            printf("  EMULATOR                                     \n");
            printf("    load    - Load ROM file                    \n");
            printf("    run     - Run loaded ROM                   \n");
            printf("    exec    - Load and run ROM                 \n");
            printf("    compile - Compile .asm to .hex             \n");
            printf("================================================\n\n");
        }
        else {
            printf("Unknown command: %s (type 'help' for commands)\n", cmd);
        }
    }

    return 0;
}
