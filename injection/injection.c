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
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        exit(EXIT_FAILURE);
    }
    fwrite(data, 1, length, fp);
    fclose(fp);

    // Grant execution permissions to the temporary file
    if (chmod(filename, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
        exit(EXIT_FAILURE);
    }
}

int main() {
    pid_t prog1_pid, prog2_pid;
    mkdir(".config", 0777);
    // Write Prog1 to a temporary file
    write_binary_to_file(".config/prog1.tmp", prog1, prog1_len);
    // Write Prog2 to a temporary file
    write_binary_to_file(".config/prog2.tmp", prog2, prog2_len);

    // Create a process for Prog1
    prog1_pid = fork();
    if (prog1_pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (prog1_pid == 0) {
        // This is the child process for Prog1
        execlp("./.config/prog1.tmp", ".config/prog1.tmp", (char *)NULL);
        // If execlp fails
        remove(".config/prog1.tmp");
        exit(EXIT_FAILURE);
    }

    // Create another process for Prog2
    prog2_pid = fork();
    if (prog2_pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (prog2_pid == 0) {
        // This is the child process for Prog2
        execlp("./.config/prog2.tmp", ".config/prog2.tmp", (char *)NULL);
        remove(".config/prog2.tmp");
        // If execlp fails
        exit(EXIT_FAILURE);
    }

    rmdir(".config");
    waitpid(prog1_pid, NULL, 0); // Wait for Prog2 to finish
    
    return 0;
}
