#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include <stdio.h>
#include <stdlib.h>

static int g_test_passes = 0;
static int g_test_failures = 0;

#define TEST_PASS()      \
    do                   \
    {                    \
        ++g_test_passes; \
    } while (0)

#define TEST_FAIL(msg)                                                     \
    do                                                                     \
    {                                                                      \
        ++g_test_failures;                                                 \
        fprintf(stderr, "[FAIL] %s (%s:%d)\n", (msg), __FILE__, __LINE__); \
        exit(EXIT_FAILURE);                                                \
    } while (0)

#define ASSERT(cond)                                                           \
    do                                                                         \
    {                                                                          \
        if (!(cond))                                                           \
        {                                                                      \
            ++g_test_failures;                                                 \
            fprintf(stderr, "[FAIL] %s (%s:%d)\n", #cond, __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        ++g_test_passes;                                                       \
    } while (0)

#define ASSERT_EQ(a, b)                                                      \
    do                                                                       \
    {                                                                        \
        long long _lhs = (long long)(a);                                     \
        long long _rhs = (long long)(b);                                     \
        if (_lhs != _rhs)                                                    \
        {                                                                    \
            ++g_test_failures;                                               \
            fprintf(stderr, "[ASSERT_EQ FAILED] %s=%lld, %s=%lld (%s:%d)\n", \
                    #a,                                                      \
                    _lhs,                                                    \
                    #b,                                                      \
                    _rhs,                                                    \
                    __FILE__,                                                \
                    __LINE__);                                               \
            exit(EXIT_FAILURE);                                              \
        }                                                                    \
        ++g_test_passes;                                                     \
    } while (0)

#define ASSERT_NE(a, b)                                                      \
    do                                                                       \
    {                                                                        \
        long long _lhs = (long long)(a);                                     \
        long long _rhs = (long long)(b);                                     \
        if (_lhs == _rhs)                                                    \
        {                                                                    \
            ++g_test_failures;                                               \
            fprintf(stderr, "[ASSERT_NE FAILED] %s=%lld, %s=%lld (%s:%d)\n", \
                    #a,                                                      \
                    _lhs,                                                    \
                    #b,                                                      \
                    _rhs,                                                    \
                    __FILE__,                                                \
                    __LINE__);                                               \
            exit(EXIT_FAILURE);                                              \
        }                                                                    \
        ++g_test_passes;                                                     \
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

#endif /* TEST_HARNESS_H */
