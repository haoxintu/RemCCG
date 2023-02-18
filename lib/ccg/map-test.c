#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
      char* key;
      int value;
} strIntMap;

int compar(const void *l, const void *r)
{
    const strIntMap *lm = l;
    const strIntMap *lr = r;
    return strcmp(lm->key, lr->key);
}

int main(int argc, char **argv)
{
    void *root = 0;

    strIntMap *a = malloc(sizeof(strIntMap));
    a->key = strdup("two\n");
    a->value = 100;
    tsearch(a, &root, compar); /* insert */

    strIntMap *find_a = malloc(sizeof(strIntMap));
    find_a->key = strdup("two\n");

    void *r = tfind(find_a, &root, compar); /* read */
    printf("%d", (*(strIntMap**)r)->value);

    return 0;
}
