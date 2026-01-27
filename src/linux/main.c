#include <stdio.h>
#include <dirent.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
    DIR *dir;
    
    struct dirent *entry;
    int entry_idx = 0;
    int cur_entry_idx = 0;

    char cur_entry_name[255] = ".";

    while (1)
    {
        dir = opendir(cur_entry_name);
        if (dir == NULL) { printf("error opening directory"); return 1; }

        int entry_amount = 0;
        while ((entry = readdir(dir)) != NULL) entry_amount++;

        printf("\033[H\033[J");
        rewinddir(dir);
        entry_idx = 0;

        if (cur_entry_idx == -1) cur_entry_idx++;
        if (cur_entry_idx == entry_amount) cur_entry_idx--;

        while ((entry = readdir(dir)) != NULL)
        {
            if (entry_idx == cur_entry_idx)
            {
                printf(" * ");
                printf("%s\n", entry->d_name);
            }
            else printf("   %s\n", entry->d_name);
            entry_idx++;
        }

        struct termios oldt, newt;
        char c;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        c = getchar();

        if (c == 'q') break;

        if (c == 'h')
        {
            char new_path[255];
            snprintf(new_path, sizeof(new_path), "%s/..", cur_entry_name);

            DIR *new_dir = opendir(new_path);

            closedir(dir);
            dir = new_dir;
            snprintf(cur_entry_name, sizeof(cur_entry_name), "%s", new_path);
            cur_entry_idx = 0;
        }
        if (c == 'j') cur_entry_idx++;
        if (c == 'k') cur_entry_idx--;
        if (c == 'l')
        {
            rewinddir(dir);
            entry_idx = 0;
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry_idx == cur_entry_idx)
                {
                    char new_path[255];
                    snprintf(new_path, sizeof(new_path), "%s/%s", cur_entry_name, entry->d_name);

                    DIR *new_dir = opendir(new_path);
                    if (new_dir)
                    {
                        closedir(dir);
                        dir = new_dir;
                        snprintf(cur_entry_name, sizeof(cur_entry_name), "%s", new_path);
                        cur_entry_idx = 0;
                    }
                    break;
                }
                entry_idx++;
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        closedir(dir);
    }
    
    printf("\033[H\033[J");

    return 0;
}
