#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 51) {
        std::cerr << "Usage: ./myfstat-cer508 <filename1> <filename2> ... (up to 50 filenames)" << std::endl;
        return 1;
    }

    struct stat file_info;
    struct rusage resource_usage;

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            std::cerr << "Failed to open file: " << argv[i] << std::endl;
            continue;
        }

        if (fstat(fd, &file_info) < 0) {
            std::cerr << "Failed to get file info: " << argv[i] << std::endl;
            close(fd);
            continue;
        }

        std::cout << "File: " << argv[i] << std::endl;
        std::cout << "Type: " << (S_ISDIR(file_info.st_mode) ? "Directory" : "Regular File") << std::endl;
        std::cout << "User ID: " << file_info.st_uid << std::endl;
        std::cout << "Group ID: " << file_info.st_gid << std::endl;
        std::cout << "Number of Links: " << file_info.st_nlink << std::endl;
        std::cout << "Last Access Time: " << std::ctime(&file_info.st_atime);
        std::cout << "Last Modification Time: " << std::ctime(&file_info.st_mtime);
        std::cout << "Last Status Change Time: " << std::ctime(&file_info.st_ctime);
        std::cout << "Size: " << file_info.st_size << " bytes" << std::endl;
        std::cout << "Inode Number: " << file_info.st_ino << std::endl;
        std::cout << std::endl;

        close(fd);
    }

    if (getrusage(RUSAGE_SELF, &resource_usage) < 0) {
        std::cerr << "Failed to get resource usage information" << std::endl;
        return 1;
    }

    std::cout << "User CPU time consumed: " << resource_usage.ru_utime.tv_sec << " seconds, "
              << resource_usage.ru_utime.tv_usec << " microseconds" << std::endl;
    std::cout << "System CPU time consumed: " << resource_usage.ru_stime.tv_sec << " seconds, "
              << resource_usage.ru_stime.tv_usec << " microseconds" << std::endl;

    return 0;
}
