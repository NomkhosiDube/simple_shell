#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stddef.h>

typedef struct {
    int cmd_buf_type;
    int status;
    char **alias;
    char **env;
    char **argv;
} info_t;

typedef struct {
    char *str;
} list_t;

/* Function declarations */
int is_chain(info_t *info, char *buf, size_t *p);
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len);
list_t *node_starts_with(list_t *list, const char *prefix, char delimiter);
char *_strdup(const char *s);
char *_strchr(const char *s, int c);
char *convert_number(int value, int base, int uppercase);
int _strcmp(const char *s1, const char *s2);
int replace_string(char **old, char *new);

int main(void)
{
    printf("Welcome to My Program!\n");

    char str [100];
  
    printf("Welcome to My Program: ");
    
    scanf("%99s", str);
    
    printf("Welcome to My Program: %s\n", str);

        
    return 0;
}

/* Function definitions */

/**
 * is_chain - Test if the current char in the buffer is a chain delimiter
 * @info: The parameter struct
 * @buf: The char buffer
 * @p: Address of the current position in buf
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int is_chain(info_t *info, char *buf, size_t *p)
{
    size_t j = *p;

    if (buf[j] == '|' && buf[j + 1] == '|') {
        buf[j] = 0;
        j++;
        info->cmd_buf_type = CMD_OR;
    } else if (buf[j] == '&' && buf[j + 1] == '&') {
        buf[j] = 0;
        j++;
        info->cmd_buf_type = CMD_AND;
    } else if (buf[j] == ';') {
        buf[j] = 0;
        info->cmd_buf_type = CMD_CHAIN;
    } else {
        return 0;
    }

    *p = j;
    return 1;
}

/**
 * check_chain - Checks if we should continue chaining based on the last status
 * @info: The parameter struct
 * @buf: The char buffer
 * @p: Address of the current position in buf
 * @i: Starting position in buf
 * @len: Length of buf
 *
 * Return: Void
 */
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
    size_t j = *p;

    if (info->cmd_buf_type == CMD_AND) {
        if (info->status) {
            buf[i] = 0;
            j = len;
        }
    }
    if (info->cmd_buf_type == CMD_OR) {
        if (!info->status) {
            buf[i] = 0;
            j = len;
        }
    }

    *p = j;
}

/**
 * replace_alias - Replaces an alias in the tokenized string
 * @info: The parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(info_t *info)
{
    int i;
    list_t *node;
    char *p;

    for (i = 0; i < 10; i++) {
        node = node_starts_with(info->alias, info->argv[0], '=');
        if (!node)
            return 0;
        free(info->argv[0]);
        p = _strchr(node->str, '=');
        if (!p)
            return 0;
        p = _strdup(p + 1);
        if (!p)
            return 0;
        info->argv[0] = p;
    }
    return 1;
}

/**
 * replace_vars - Replaces vars in the tokenized string
 * @info: The parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *info)
{
    int i = 0;
    list_t *node;

    for (i = 0; info->argv[i]; i++) {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
            continue;

        if (!_strcmp(info->argv[i], "$?")) {
            replace_string(&(info->argv[i]),
                _strdup(convert_number(info->status, 10, 0)));
            continue;
        }
        if (!_strcmp(info->argv[i], "$$")) {
            replace_string(&(info->argv[i]),
                _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(info->env, &info->argv[i][1], '=');
        if (node) {
            replace_string(&(info->argv[i]),
                _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&info->argv[i], _strdup(""));
    }
    return 0;
}

/**
 * replace_string - Replaces string
 * @old: Address of the old string
 * @new: New string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
    free(*old);
    *old = new;
    return 1;
}

