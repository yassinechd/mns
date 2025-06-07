#include "../include/minishell.h"
#include <limits.h>
#include <errno.h>


int exec_builtin(char **args, char ***env)
{
    if (!args || !args[0])
        return 1;

    if (ft_strcmp(args[0], "echo") == 0)
    {
        printf("\n --- echo --- \n");
        return ft_echo(args);
    }
        
    if (ft_strcmp(args[0], "cd") == 0)
        return ft_cd(args, env);
    if (ft_strcmp(args[0], "pwd") == 0)
    {
        printf("\n --- echo --- \n");
        return ft_pwd(args);
    }
        
    if (ft_strcmp(args[0], "export") == 0)
        return ft_export(args, env);
    if (ft_strcmp(args[0], "unset") == 0)
        return ft_unset(args, env);
    if (ft_strcmp(args[0], "env") == 0)
        return ft_env(args, *env);
    if (ft_strcmp(args[0], "exit") == 0)
        return ft_exit(args);

    return 1;
}



int ft_echo(char **args) {
    int newline = 1;
    int i = 1;

    while (args[i] && args[i][0] == '-' &&  args[i][1] == 'n') {
        int j = 2;
        while (args[i][j] == 'n')
            j++;
        if (args[i][j] != '\0')
            break;
        newline = 0;
        i++;
    }

    while (args[i]) {
        ft_printf("%s", args[i]);
        if (args[i + 1])
            ft_printf(" ");
        i++;
    }
    if (newline)
        ft_printf("\n");
    return 0;
}


int ft_cd(char **args, char ***env) {
    char *path = args[1];
    char cwd[100];
    char oldpwd[100];

    if (getcwd(oldpwd, sizeof(oldpwd)) == NULL) {
        perror("cd");
        return 1;
    }

    if (!path) {
        path = getenv("HOME");
        if (!path) {
            ft_printf("cd: HOME not set\n");
            return 1;
        }
    }
    else if (ft_strcmp(path, "-") == 0) {
        path = getenv("OLDPWD");
        if (!path) {
            ft_printf("cd: OLDPWD not set\n");
            return 1;
        }
        ft_printf("%s\n", path);
    }

    if (chdir(path) != 0) {
        ft_printf("cd: %s: %s\n", path, strerror(errno));
        return 1;
    }

    add_or_update_env("OLDPWD", oldpwd, env);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        add_or_update_env("PWD", cwd, env);
    }

    return 0;
}

int ft_pwd(char **args)
{
    char cwd[100];
    (void)args;
    if (getcwd(cwd, sizeof(cwd))) {
        ft_printf("%s\n", cwd);
        return 0;
    }
    perror("pwd");
    return 1;
}

int	ft_exit(char **args)
{
    int	status;
    int	i;

    ft_printf("exit\n");
    if (!args[1])
        exit(0);

    i = 0;
    if (args[1][i] == '-' || args[1][i] == '+')
        i++;
    while (args[1][i])
    {
        if (!ft_isdigit(args[1][i]))
        {
            ft_printf("exit: %s: numeric argument required\n", args[1]);
            exit(255);
        }
        i++;
    }
    if (args[2])
    {
        ft_printf("exit: too many arguments\n");
        return (1);
    }
    status = ft_atoi(args[1]);
    exit((unsigned char)status);
    return (status);
}

int add_or_update_env(char *key, char *value, char ***env) {
    int i = 0;
    size_t key_len = ft_strlen(key);
    char *new_entry;

    new_entry = malloc(ft_strlen(key) + ft_strlen(value) + 2);
    if (!new_entry)
        return -1;
    ft_strlcpy(new_entry, key, ft_strlen(key) + 1);
    ft_strlcat(new_entry, "=", ft_strlen(key) + 2);
    ft_strlcat(new_entry, value, ft_strlen(key) + ft_strlen(value) + 2);

    while ((*env)[i]) {
        if (ft_strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=') {
            free((*env)[i]);
            (*env)[i] = new_entry;
            return 0;
        }
        i++;
    }

    int count = 0;
    while ((*env)[count])
        count++;

    char **new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env) {
        free(new_entry);
        return -1;
    }

    i = 0;
    while (i < count) {
        new_env[i] = (*env)[i];
        i++;
    }
    new_env[count] = new_entry;
    new_env[count + 1] = NULL;

    free(*env);
    *env = new_env;

    return 0;
}


int cmp_env(const void *a, const void *b)
{
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return ft_strcmp((char *)s1, (char *)s2);
}

int is_valid_identifier(const char *str) {
    int i = 0;
    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return 0;
    while (str[i] && str[i] != '=') {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

int ft_export(char **args, char ***env) {
    int i = 1;

    if (!args[1]) {
        int count = 0;
        while ((*env)[count])
            count++;
        char **sorted = malloc(sizeof(char *) * (count + 1));
        if (!sorted)
            return 1;
        int j = 0;
        while (j < count) {
            sorted[j] = (*env)[j];
            j++;
        }
        sorted[count] = NULL;
        qsort(sorted, count, sizeof(char *), cmp_env);
        j = 0;
        while (j < count) {
            char *eq = ft_strchr(sorted[j], '=');
            if (eq) {
                ft_printf("declare -x ");
                int k = 0;
                while (sorted[j][k] && sorted[j][k] != '=')
                    ft_printf("%c", sorted[j][k++]);
                ft_printf("=\"%s\"\n", eq + 1);
            } else {
                ft_printf("declare -x %s\n", sorted[j]);
            }
            j++;
        }
        free(sorted);
        return 0;
    }

    while (args[i]) {
        char *equal_sign = ft_strchr(args[i], '=');
        char key[1024] = {0};
        int k = 0;
        if (equal_sign) {
            while (args[i][k] && &args[i][k] != equal_sign && k < 1023) {
                key[k] = args[i][k];
                k++;
            }
            key[k] = '\0';
        } else {
            k = 0;
            while (args[i][k] && k < 1023) {
                key[k] = args[i][k];
                k++;
            }
            key[k] = '\0';
        }
        if (!is_valid_identifier(key)) {
            ft_printf("export: `%s': not a valid identifier\n", args[i]);
            i++;
            continue;
        }
        if (equal_sign) {
            char *value = equal_sign + 1;
            if (add_or_update_env(key, value, env) != 0) {
                perror("export");
                return 1;
            }
        } else {
            add_or_update_env(key, "", env);
        }
        i++;
    }
    return 0;
}

int remove_env(char *key, char ***env) {
    int i = 0, j = 0;
    size_t key_len = ft_strlen(key);

    while ((*env)[i]) {
        if (ft_strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=') {
            free((*env)[i]);
            i++;
            break;
        }
        i++;
    }

    while ((*env)[i]) {
        (*env)[j++] = (*env)[i++];
    }
    (*env)[j] = NULL;

    return 0;
}

int ft_unset(char **args, char ***env) {
    int i = 1;

    if (!args[1]) {
        ft_printf("unset: not enough arguments\n");
        return 1;
    }

    while (args[i]) {
        if (remove_env(args[i], env) != 0) {
            ft_printf("unset: `%s': not a valid identifier\n", args[i]);
        }
        i++;
    }
    return 0;
}

int ft_env(char **args, char **env) {
    int i = 0;


    if (args[1]) {
        ft_printf("env: '%s': No such file or directory\n", args[1]);
        return 127;
    }

    while (env[i]) {
        if (ft_strchr(env[i], '='))
            ft_printf("%s\n", env[i]);
        i++;
    }
    return 0;
}