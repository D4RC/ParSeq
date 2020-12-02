#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE ( sizeof (struct inotify_event) )
#define BUF_LEN    ( 1024 * (  EVENT_SIZE + 16  )  )

int main()
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[BUF_LEN];

    fd = inotify_init();

    if (fd < 0) 
    {
        perror("inotify_init");
    }

    wd = inotify_add_watch(fd, "./sequences", IN_CREATE | IN_DELETE);

    length = read(fd, buffer, BUF_LEN);

    if (length < 0) 
    {
        perror("read");
    }

    while (i < length)
    {
        struct inotify_event *event = (struct inotify_event *) &buffer[i];

        if (event -> len) 
        {
            if (event->mask & IN_CREATE) 
            {
                if (event->mask & IN_ISDIR)
                {
                    printf("New directory %s created\n", event->name);
                }
                else
                {
                    printf("New file %s created\n", event->name);
                }
            }
            else if (event->mask & IN_DELETE)
            {
                if(event->mask & IN_ISDIR)
                {
                    printf("Directory %s deleted", event->name);
                }
                else
                {
                    printf("File %s deleted\n", event->name);
                }   
            }
        }
        i += EVENT_SIZE + event->len;
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}