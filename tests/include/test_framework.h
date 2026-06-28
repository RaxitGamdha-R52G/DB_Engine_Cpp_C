#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>

static int g_test_passes = 0;
static int g_test_failures = 0;

#define RUN_TEST(fn)                       \
    do                                     \
    {                                      \
        if ((fn)())                        \
        {                                  \
            ++g_test_passes;               \
            printf("[PASS] %s\n", #fn);    \
        }                                  \
        else                               \
        {                                  \
            ++g_test_failures;             \
            fprintf(stderr,                \
                    "[FAIL] %s (%s:%d)\n", \
                    #fn,                   \
                    __FILE__,              \
                    __LINE__);             \
        }                                  \
    } while (0)

static inline int test_summary(void)
{
    printf("\n=== Test Summary ===\n");
    printf("Passed : %d\n", g_test_passes);
    printf("Failed : %d\n", g_test_failures);

    return (g_test_failures == 0)
               ? EXIT_SUCCESS
               : EXIT_FAILURE;
}

#endif /* TEST_FRAMEWORK_H */