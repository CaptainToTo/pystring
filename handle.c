#include "pystring.h"

int main(void) {
    char *str1 = "Hello";
    char *str2 = "World";
    char *str3 = "foo";
    char *str4 = "bar";

    if (str_eq(str1, str1)) printf("String is equal to itself\n");

    if (!str_eq(str1, str2)) printf("Different strings aren't equal\n");

    if (str_lt(str1, str2)) printf("Hello < World\n");

    if (str_gt(str3, str4)) printf("foo > bar\n");

    printf("%d matching characters between Hello and World\n", match(str1, str2));

    printf("first instance of 'rld' in World starts at index %d\n", find(str2, "rld"));

    if (contains(str4, "ba")) printf("bar contains 'ba'\n");

    if (!contains(str3, "ba")) printf("foo doesn't contain 'ba'\n\n");

    // ===============================

    char *str5 = "This is a complete sentence.";
    unsigned long len;
    char **tokens = split(str5, ' ', &len);
    for (unsigned long i = 0; i < len; i++) {
        printf("%s\n", tokens[i]);
    }
    printf("\n");

    unsigned long len2;
    char *joined = join(tokens, len, '/', &len2);
    printf("joined with '/': %s\n", joined);

    char *replaced = replace(joined, "/", " ", NULL);
    printf("replaced '/' with ' ': %s\n", replaced);

    char *str1_up = upper_dup(str1);
    printf("Hello: %s\n", str1_up);

    delete_strs(&tokens, len);
    free(joined);
    free(replaced);
    free(str1_up);
    return 0;
}
