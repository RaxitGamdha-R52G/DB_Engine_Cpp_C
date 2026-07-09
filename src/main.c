#include <stdio.h>

#include "student_table.h"

static void print_student_callback(const Student *student,
                                   void *userdata)
{
    (void)userdata;
    student_display(student);
}

// Predicate Functions
static bool min_gpa(const Student *student,
                    void *ctx)
{
    float min = *(float *)ctx;
    return student->gpa >= min;
}

static bool active_only(const Student *student,
                        void *ctx)
{
    (void)ctx;
    return student->is_active;
}

static bool by_age(const Student *student,
                   void *ctx)
{
    uint32_t age = *(uint32_t *)ctx;
    return student->age >= age;
}

// Custom Comparator
static int compare_by_gpa(const void *a,
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

    if (s1->gpa < s2->gpa)
        return -1;

    if (s1->gpa > s2->gpa)
        return 1;

    return 0;
}

int main(void)
{
    StudentTable *tbl = student_table_create();

    if (tbl == NULL)
    {
        printf("Failed to create student table.\n");
        return 1;
    }

    Student student1 = make_student(
        "923456789012345",
        "Alice in Borderland",
        "alice@borderland.com",
        24,
        3.9f,
        true);

    Student student2 = make_student(
        "123456789012346",
        "Borderland",
        "borderland@test.com",
        20,
        3.8f,
        true);

    Student student3 = make_student(
        "123456789012347",
        "Charlie",
        "charlie@test.com",
        25,
        3.4f,
        true);

    student_print_status("Insert 1",
                         student_insert(tbl, &student1));

    student_print_status("Insert 2",
                         student_insert(tbl, &student2));

    student_print_status("Insert 3",
                         student_insert(tbl, &student3));

    printf("\n========== ALL STUDENTS ==========\n\n");
    student_display_all(tbl);

    printf("\n========== FIND BY ID ==========\n\n");

    const Student *found =
        student_find_by_id(tbl, "123456789012346");

    if (found != NULL)
    {
        student_display(found);
    }

    printf("\n========== SORT BY ID ==========\n\n");

    student_sort_by_id(tbl);
    student_display_all(tbl);

    printf("\n========== SORT BY NAME ==========\n\n");

    student_sort_by_name(tbl);
    student_display_all(tbl);

    printf("\n========== SORT BY GPA (Generic Sort) ==========\n\n");

    student_sort(tbl, compare_by_gpa);
    student_display_all(tbl);

    printf("\n========== FOREACH ==========\n\n");

    student_foreach(tbl,
                    print_student_callback,
                    NULL);

    const Student *results[MAX_RECORDS];
    uint32_t count;

    printf("\n========== FILTER : GPA >= 3.5 ==========\n\n");

    float min = 3.5f;

    count = student_filter(tbl,
                           min_gpa,
                           &min,
                           results,
                           MAX_RECORDS);

    for (uint32_t i = 0; i < count; i++)
    {
        student_display(results[i]);
    }

    printf("\n========== FILTER : ACTIVE STUDENTS ==========\n\n");

    count = student_filter(tbl,
                           active_only,
                           NULL,
                           results,
                           MAX_RECORDS);

    for (uint32_t i = 0; i < count; i++)
    {
        student_display(results[i]);
    }

    printf("\n========== FILTER : AGE >= 22 ==========\n\n");

    uint32_t age = 22;

    count = student_filter(tbl,
                           by_age,
                           &age,
                           results,
                           MAX_RECORDS);

    for (uint32_t i = 0; i < count; i++)
    {
        student_display(results[i]);
    }

    printf("\n========== DELETE ==========\n\n");

    student_print_status("Delete",
                         student_delete(tbl, "123456789012346"));

    student_display_all(tbl);

    printf("\nStudent Count : %u\n",
           student_count(tbl));

    student_table_destroy(tbl);

    return 0;
}