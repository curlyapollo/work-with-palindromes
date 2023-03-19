#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int max_size = 50000;

int main(int argc, char *argv[]) {
    int fd1; // 1st channel.
    int fd2; // 2nd channel
    char str[max_size]; // Checking string.
    mknod("read", S_IFIFO | 0666, 0);
    pid_t pid;
    int sz, proc;
    switch(pid = fork()) { // Doing fork.
        case -1:
            perror("fork"); // There was an error.
            exit(1); // Exit from the parent process with code 1.
        case 0:
            if ((proc = open(argv[1], O_RDONLY)) < 0) {
                printf("Error with the opening of file.\n"); // Message about the error.
                exit(-1); // Exit from the parent process with code -1.
            }
            if((fd1 = open("read", O_WRONLY)) < 0) {
                printf("Error with the opening FIFO for writing\n"); // Message about the error.
                exit(-1); // Exit from the parent process with code -1.
            }
            sz = read(proc, str, max_size); // Reading from the file.
            if (sz == -1) {
                printf("Error with the reading of file.\n"); // Message about the error.
                exit(-1); // Exit from the parent process with code -1.
            }
            sz = write(fd1, str, sz);
            close(fd1); // Closing of the channel.
            exit(0); // Exit from the parent process.
        default:
            if((fd1 = open("read", O_RDONLY)) < 0) {
                printf("Error with the opening FIFO for reading\n"); // Message about the error.
                exit(-1); // Exit from the parent process with code -1.
            }
            sz = read(fd1, str, max_size);
            close(fd1); // Message about the error.
    }
    int n = 0; // Length of the input string.
    while (str[n] != 0) {
        n++;
    }
    int flag = 1; // Binary flag.
    for (int i = 0; i < n / 2; ++i) {
        if (str[i] != str[n - i - 1]) { // In palindromes i-th element equals 'n - i'-th element.
            flag = -1; // Not palindrome.
            break; // The end of the checking.
        }
    }
    char res[max_size]; // Result string {"YES", "NO"}.
    if (flag == 1) {
        sz = 3; // Size of "YES" is 3.
        res[0] = 'Y';
        res[1] = 'E';
        res[2] = 'S';
    } else {
        sz = 2; // Size of "NO" is 2.
        res[0] = 'N';
        res[1] = 'O';
    }
    char buff[max_size];
    mknod("write", S_IFIFO | 0666, 0);
    switch(pid = fork()) { // Doing fork.
        case -1:
            perror("fork"); // There was an error.
            exit(1); // Exit from the parent process with code 1.
        case 0:
            if ((proc = open(argv[2], O_WRONLY)) < 0) { // First stream outputs answer in file.
                printf("Error with the opening of file.\n"); // Filename is 1st argument of command line.
                exit(-1); // Exit from the parent process with code -1.
            }
            if((fd2 = open("write", O_RDONLY)) < 0) {
                printf("Error with the opening FIFO for reading\n"); // Message about the error.
                exit(-1); // Exit from the parent process with code -1.
            }
            sz = read(fd2, buff, max_size); // Reading from the file.
            if (write(proc, buff, sz) == -1) {
                printf("Error with the reading of file.\n"); // Message about the error.
                exit(-1); // Exit from the parent process with code -1.
            }
            close(fd2); // Closing of the channel.
        default:
            if((fd2 = open("write", O_WRONLY)) < 0) {
                printf("Error with the opening FIFO for writing\n"); // Message about the error.
                exit(-1); // Exit from the parent process with code -1.
            }
            sz = write(fd2, res, sz);
            close(fd2); // Closing of the channel.
    }
    return 0;
}
