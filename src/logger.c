#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/syscall.h>
#include<ctype.h>
#include<string.h>
#include<sys/wait.h>
#include"find.h"

struct linux_dirent64 {
    unsigned long long d_ino;
    long long          d_off;
    unsigned short     d_reclen;
    unsigned char      d_type;
    char               d_name[];
};

struct record{
    char pname[120];
    char state[20];
};

void add_log(char* path);

void logger()
{
    int fd, n;
    int i = 0, brige[2];

    char* buf = malloc(4096);
    struct linux_dirent64* org_data;

    fd = open("/proc", O_RDONLY | O_DIRECTORY, 0777);
    if (fd < 0) {
        perror("open");
        return;
    }

    n = syscall(SYS_getdents64, fd, buf, 4096);
    if (n <= 0) {
        free(buf);
        close(fd);
        return;
    }

    while (i < n)
    {
        pipe(brige);

        org_data = (struct linux_dirent64*)(buf + i);

        if (isdigit(org_data->d_name[0]))
        {
            char pid[64];
            strncpy(pid, org_data->d_name, sizeof(pid) - 1);
            pid[sizeof(pid) - 1] = '\0';

            pid_t p = fork();

            if (p > 0)
            {
                close(brige[0]);

                write(brige[1], pid, strlen(pid));
                close(brige[1]);

                wait(NULL);
            }
            else if (p == 0)
            {
                close(brige[1]);

                char t_buf[32];
                int r = read(brige[0], t_buf, sizeof(t_buf) - 1);
                if (r < 0) r = 0;
                t_buf[r] = '\0';

                close(brige[0]);

                char tmp[64] = "/proc/";
                strcat(tmp, t_buf);
                strcat(tmp, "/status");

                add_log(tmp);

                free(buf);  
                close(fd);

                _exit(0);
            }
        }

        i += org_data->d_reclen;
    }

    free(buf);
    close(fd);
}

void add_log(char* path)
{
    int F = open("./build/log.txt", O_CREAT | O_RDWR | O_APPEND, 0777);
    if (F < 0) return;

    struct record user;

    char *name  = find(path, "Name:");
    char *state = find(path, "State:");

    if (name && state)
    {
        strncpy(user.pname, name, sizeof(user.pname) - 1);
        user.pname[sizeof(user.pname) - 1] = '\0';

        strncpy(user.state, state, sizeof(user.state) - 1);
        user.state[sizeof(user.state) - 1] = '\0';

        write(F, user.pname, strlen(user.pname));
        write(F, "\t", 1);
        write(F, user.state, strlen(user.state));
        write(F, "\n", 1);
    }

    close(F);
}

int main()
{
    logger();
}
