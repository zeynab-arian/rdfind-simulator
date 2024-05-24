#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_PATH 1024
#define MAX_THREADS 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;

typedef struct {
    char path[MAX_PATH];
    off_t size;
} FileInfo;

FileInfo files[MAX_THREADS];
int file_count = 0;
int total_files = 0;
int deleted_files_count = 0;
off_t initial_size = 0;
off_t final_size = 0;

void add_file_size(const char* path, off_t* size) {
    struct stat st;
    if (stat(path, &st) == 0) {
        *size += st.st_size;
    }
}

void* process_file(void* arg) {
    FileInfo* file = (FileInfo*)arg;
    printf("Processing file: %s\n", file->path);

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].path, file->path) == 0 && i != file_count) {
            printf("Duplicate file found: %s\n", file->path);
            if (unlink(file->path) == 0) {
                printf("File removed: %s\n", file->path);
                deleted_files_count++;
                add_file_size(file->path, &final_size);
            } else {
                perror("unlink");
            }
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void process_directory(const char* dir_path) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(dir_path)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            pthread_mutex_lock(&mutex);
            strcpy(files[file_count].path, dir_path);
            strcat(files[file_count].path, "/");
            strcat(files[file_count].path, entry->d_name);
            add_file_size(files[file_count].path, &initial_size);
            file_count++;
            total_files++;
            pthread_mutex_unlock(&mutex);
        }
    }
    closedir(dir);
}

void* process_subdirectory(void* arg) {
    char* dir_path = (char*)arg;
    process_directory(dir_path);

    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < file_count; i++) {
        pthread_create(&threads[i], NULL, process_file, (void*)&files[i]);
    }

    for (int i = 0; i < file_count; i++) {
        pthread_join(threads[i], NULL);
    }

    char log_filename[MAX_PATH];
    snprintf(log_filename, sizeof(log_filename), "%s.log", dir_path);
    FILE* log_file = fopen(log_filename, "w");
    if (log_file == NULL) {
        perror("fopen");
        return NULL;
    }
    for (int i = 0; i < file_count; i++) {
        fprintf(log_file, "Processed file: %s\n", files[i].path);
    }
    fclose(log_file);

    return NULL;
}

void traverse_directories(const char* base_path) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(base_path)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[MAX_PATH];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);
            pid_t pid = fork();
            if (pid == 0) { // فرآیند فرزند
                process_subdirectory((void*)path);
                exit(0);
            } else if (pid < 0) {
                perror("fork");
            }
        }
    }
    closedir(dir);
}

int main() {
    char base_path[MAX_PATH];

    printf("Enter the directory path: ");
    if (fgets(base_path, sizeof(base_path), stdin) != NULL) {
        size_t len = strlen(base_path);
        if (len > 0 && base_path[len-1] == '\n') {
            base_path[len-1] = '\0'; // حذف newline character
        }
    }

    sem_init(&sem, 0, 1);

    traverse_directories(base_path);

    while (wait(NULL) > 0); // انتظار برای اتمام تمامی فرآیندهای فرزند

    sem_destroy(&sem);

    // نمایش خروجی
    printf("Total files processed: %d\n", total_files);
    printf("Total duplicate files removed: %d\n", deleted_files_count);
    printf("Initial size: %ld bytes\n", initial_size);
    printf("Final size: %ld bytes\n", final_size);
    printf("Space saved: %ld bytes\n", initial_size - final_size);

    return 0;
}
