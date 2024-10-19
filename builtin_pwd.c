#include "main.h"

void handle_pwd_command(void)
{
    char *tmp_pwd;

    tmp_pwd = getcwd(NULL, BUFSIZ);
    printf("%s\n", tmp_pwd);
    free(tmp_pwd);
}