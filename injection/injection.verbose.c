#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

// Include binaries converted to C data
extern unsigned char prog1[];
extern unsigned int prog1_len;
extern unsigned char prog2[];
extern unsigned int prog2_len;

// Function to write binaries into temporary files
void write_binary_to_file(const char *filename, unsigned char *data, unsigned int length) {
    fprintf(stderr, "[INFO] Writing binary data to file: %s\n", filename);

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "[ERROR] Failed to open file %s for writing.\n", filename);
        exit(EXIT_FAILURE);
    }

    size_t written = fwrite(data, 1, length, fp);
    if (written != length) {
        fprintf(stderr, "[ERROR] Failed to write complete data to file %s.\n", filename);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    fprintf(stderr, "[INFO] Successfully wrote %u bytes to %s.\n", length, filename);

    // Grant execution permissions to the temporary file
    if (chmod(filename, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
        fprintf(stderr, "[ERROR] Failed to set executable permissions for %s.\n", filename);
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "[INFO] Set executable permissions for %s.\n", filename);
}

int main() {
    pid_t prog1_pid, prog2_pid;

    fprintf(stderr, "[INFO] Creating directory .config.\n");
    if (mkdir(".config", 0777) != 0) {
        fprintf(stderr, "[ERROR] Failed to create directory .config.\n");
        exit(EXIT_FAILURE);
    }

    // Write Prog1 to a temporary file
    write_binary_to_file(".config/prog1.tmp", prog1, prog1_len);
    // Write Prog2 to a temporary file
    write_binary_to_file(".config/prog2.tmp", prog2, prog2_len);

    // Create a process for Prog1
    fprintf(stderr, "[INFO] Forking process for Prog1.\n");
    prog1_pid = fork();
    if (prog1_pid < 0) {
        fprintf(stderr, "[ERROR] Fork failed for Prog1.\n");
        exit(EXIT_FAILURE);
    }

    if (prog1_pid == 0) {
        // This is the child process for Prog1
        fprintf(stderr, "[INFO] Executing ./.config/prog1.tmp.\n");
        execlp("./.config/prog1.tmp", ".config/prog1.tmp", (char *)NULL);
        // If execlp fails
        fprintf(stderr, "[ERROR] Failed to execute ./.config/prog1.tmp.\n");
        remove(".config/prog1.tmp");
        exit(EXIT_FAILURE);
    }

    // Create another process for Prog2
    fprintf(stderr, "[INFO] Forking process for Prog2.\n");
    prog2_pid = fork();
    if (prog2_pid < 0) {
        fprintf(stderr, "[ERROR] Fork failed for Prog2.\n");
        exit(EXIT_FAILURE);
    }

    if (prog2_pid == 0) {
        // This is the child process for Prog2
        fprintf(stderr, "[INFO] Executing ./.config/prog2.tmp.\n");
        execlp("./.config/prog2.tmp", ".config/prog2.tmp", (char *)NULL);
        fprintf(stderr, "[ERROR] Failed to execute ./.config/prog2.tmp.\n");
        remove(".config/prog2.tmp");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "[INFO] Removing directory .config.\n");
    if (rmdir(".config") != 0) {
        fprintf(stderr, "[ERROR] Failed to remove directory .config.\n");
    }

    fprintf(stderr, "[INFO] Waiting for Prog1 to finish.\n");
    waitpid(prog1_pid, NULL, 0);

    fprintf(stderr, "[INFO] Waiting for Prog2 to finish.\n");
    waitpid(prog2_pid, NULL, 0);

    fprintf(stderr, "[INFO] Program completed successfully.\n");
    return 0;
}
