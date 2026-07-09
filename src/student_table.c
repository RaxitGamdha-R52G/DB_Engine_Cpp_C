#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "student_table.h"

#define ID_COL 12
#define NAME_COL 20
#define EMAIL_COL 30

struct StudentTable
{
    Student *records[MAX_RECORDS];
    uint32_t count;
};

// Internal Helper Functions
static void print_column(const char *str, int width)
{
    if (str == NULL)
    {
        printf("%-*s", width, "");
        return;
    }

    size_t len = strlen(str);

    if (len <= (size_t)width)
    {
        printf("%-*s", width, str);
    }
    else
    {
        printf("%.*s...", width - 3, str);
    }
}

static void display_callback(const Student *student,
                             void *userdata)
{
    (void)userdata;

    print_column(student->student_id, ID_COL);
    printf(" ");

    print_column(student->name, NAME_COL);
    printf(" ");

    print_column(student->email, EMAIL_COL);
    printf(" ");

    printf("%-4u %-5.2f %-6s\n",
           student->age,
           student->gpa,
           student->is_active ? "Yes" : "No");
}

static bool has_id(const Student *student,
                   void *ctx)
{
    return strncmp(student->student_id,
                   (const char *)ctx,
                   STUDENT_ID_LEN) == 0;
}

static bool has_name(const Student *student,
                     void *ctx)
{
    return strncmp(student->name,
                   (const char *)ctx,
                   STUDENT_NAME_LEN) == 0;
}

static int compare_student_id(const void *a,
                              const void *b)
{
    const Student *s1 = *(const Student *const *)a;
    const Student *s2 = *(const Student *const *)b;

    if (s1 == NULL && s2 == NULL)
        return 0;

    if (s1 == NULL)
        return 1;

    if (s2 == NULL)
        return -1;

    return strncmp(s1->student_id,
                   s2->student_id,
                   STUDENT_ID_LEN);
}

static int compare_student_name(const void *a,
                                const void *b)
{
    const Student *s1 = *(const Student *const *)a;
    const Student *s2 = *(const Student *const *)b;

    if (s1 == NULL && s2 == NULL)
        return 0;

    if (s1 == NULL)
        return 1;

    if (s2 == NULL)
        return -1;

    return strncmp(s1->name,
                   s2->name,
                   STUDENT_NAME_LEN);
}

// Table Functions
StudentTable *student_table_create(void)
{
    return calloc(1, sizeof(StudentTable));
}

void student_table_destroy(StudentTable *tbl)
{
    if (tbl == NULL)
        return;

    for (uint32_t i = 0; i < MAX_RECORDS; i++)
    {
        free(tbl->records[i]);
        tbl->records[i] = NULL;
    }

    tbl->count = 0;

    free(tbl);
}

// Generic Algorithms
void student_foreach(const StudentTable *tbl,
                     student_foreach_fn fn,
                     void *userdata)
{
    if (!tbl || !fn)
        return;

    for (uint32_t i = 0; i < MAX_RECORDS; ++i)
    {
        if (tbl->records[i] != NULL)
            fn(tbl->records[i], userdata);
    }
}

uint32_t student_filter(const StudentTable *tbl,
                        student_predicate_fn predicate,
                        void *ctx,
                        const Student **out,
                        uint32_t max)
{
    if (tbl == NULL ||
        predicate == NULL ||
        out == NULL ||
        max == 0)
    {
        return 0;
    }

    uint32_t found = 0;

    for (uint32_t i = 0;
         i < MAX_RECORDS && found < max;
         i++)
    {
        if (tbl->records[i] == NULL)
            continue;

        if (predicate(tbl->records[i], ctx))
        {
            out[found++] = tbl->records[i];
        }
    }

    return found;
}

const Student *student_find_if(const StudentTable *tbl,
                               student_predicate_fn predicate,
                               const void *ctx)
{
    if (!tbl || !predicate)
        return NULL;

    for (uint32_t i = 0; i < MAX_RECORDS; i++)
    {
        if (tbl->records[i] == NULL)
            continue;

        if (predicate(tbl->records[i], ctx))
            return tbl->records[i];
    }

    return NULL;
}

void student_sort(StudentTable *tbl,
                  int (*cmp)(const void *, const void *))
{
    if (tbl == NULL)
        return;

    qsort(tbl->records,
          MAX_RECORDS,
          sizeof(Student *),
          cmp);
}

// CRUD Functions
StudentStatus student_insert(StudentTable *tbl, const Student *student)
{
    if (tbl == NULL || student == NULL)
        return STUDENT_ERR_INVALID_ARGUMENT;

    if (!student_validate(student))
        return STUDENT_ERR_VALIDATE;

    if (tbl->count >= MAX_RECORDS)
        return STUDENT_ERR_TABLE_FULL;

    if (student_find_by_id(tbl, student->student_id))
        return STUDENT_ERR_ALREADY_EXISTS;

    Student *copy = malloc(sizeof(Student));

    if (copy == NULL)
        return STUDENT_ERR_OUT_OF_MEMORY;

    *copy = *student;

    for (uint32_t i = 0; i < MAX_RECORDS; ++i)
    {
        if (tbl->records[i] == NULL)
        {
            tbl->records[i] = copy;
            ++tbl->count;

            copy = NULL;
            break;
        }
    }

    free(copy);

    return STUDENT_OK;
}

StudentStatus student_delete(StudentTable *tbl, const char *id)
{
    if (tbl == NULL || id == NULL)
        return STUDENT_ERR_INVALID_ARGUMENT;

    if (tbl->count == 0)
        return STUDENT_ERR_TABLE_EMPTY;

    uint32_t found = 0;

    for (uint32_t i = 0; i < MAX_RECORDS && found < tbl->count; ++i)
    {
        if (tbl->records[i] == NULL)
            continue;

        ++found;

        if (strncmp(tbl->records[i]->student_id, id, STUDENT_ID_LEN) == 0)
        {
            free(tbl->records[i]);
            tbl->records[i] = NULL;
            --tbl->count;
            return STUDENT_OK;
        }
    }

    return STUDENT_ERR_NOT_FOUND;
}

StudentStatus student_update(StudentTable *tbl, const char *id, const Student *new_data)
{
    if (tbl == NULL || id == NULL || new_data == NULL)
        return STUDENT_ERR_INVALID_ARGUMENT;

    if (!student_validate(new_data))
        return STUDENT_ERR_VALIDATE;

    uint32_t found = 0;

    Student *copy = malloc(sizeof(*copy));

    if (copy == NULL)
        return STUDENT_ERR_OUT_OF_MEMORY;

    *copy = *new_data;

    for (uint32_t i = 0; i < MAX_RECORDS && found < tbl->count; ++i)
    {
        if (tbl->records[i] == NULL)
            continue;

        ++found;

        if (strncmp(tbl->records[i]->student_id, id, STUDENT_ID_LEN) == 0)
        {
            free(tbl->records[i]);
            tbl->records[i] = copy;

            return STUDENT_OK;
        }
    }

    free(copy);

    return STUDENT_ERR_NOT_FOUND;
}

const Student *student_find_by_id(const StudentTable *tbl,
                                  const char *id)
{
    if (tbl == NULL || id == NULL)
        return NULL;

    return student_find_if(tbl, has_id, id);
}

const Student *student_find_by_name(const StudentTable *tbl,
                                    const char *name)
{
    if (tbl == NULL || name == NULL)
        return NULL;

    return student_find_if(tbl, has_name, name);
}

// Display Function
void student_display_all(const StudentTable *tbl)
{
    if (tbl == NULL)
    {
        printf("Student table is NULL.\n");
        return;
    }

    if (tbl->count == 0)
    {
        printf("No students.\n");
        return;
    }

    printf("---------------------------------------------------------------------------------------\n");
    printf("%-12s %-20s %-30s %-4s %-5s %-6s\n",
           "ID", "Name", "Email", "Age", "GPA", "Active");
    printf("---------------------------------------------------------------------------------------\n");

    student_foreach(tbl, display_callback, NULL);

    printf("---------------------------------------------------------------------------------------\n");
}

uint32_t student_count(const StudentTable *tbl)
{
    if (tbl == NULL)
        return 0;

    return tbl->count;
}

// Sorting Functions
void student_sort_by_id(StudentTable *tbl)
{
    student_sort(tbl, compare_student_id);
}

void student_sort_by_name(StudentTable *tbl)
{
    student_sort(tbl, compare_student_name);
}

// Status Functions
const char *student_status_str(StudentStatus status)
{
    static const char *table[] =
        {
            "SUCCESS",
            "INVALID ARGUMENT",
            "NOT FOUND",
            "ALREADY EXISTS",
            "TABLE FULL",
            "TABLE EMPTY",
            "OUT OF MEMORY",
            "VALIDATION FAILED"};

    if (status < 0 ||
        status >= (sizeof(table) / sizeof(table[0])))
        return "UNKNOWN ERROR";

    return table[status];
}

void student_print_status(const char *operation, StudentStatus status)
{
    printf("%-10s : %s\n", operation, student_status_str(status));
}