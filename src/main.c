#include <stdio.h>
#include <string.h>

#include "student_table.h"

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

    student_print_status("Insert 1", student_insert(tbl, &student1));
    student_print_status("Insert 2", student_insert(tbl, &student2));
    student_print_status("Insert 3", student_insert(tbl, &student3));

    printf("\n=== Students ===\n");
    student_display_all(tbl);

    const Student *found =
        student_find_by_id(tbl, "123456789012346");

    if (found != NULL)
    {
        printf("\nStudent Found:\n");
        student_display(found);
    }

    student_print_status("Delete", student_delete(tbl, "123456789012346"));

    printf("\n=== After Delete ===\n");
    student_display_all(tbl);

    student_sort_by_id(tbl);

    printf("\n=== Sorted by ID ===\n");
    student_display_all(tbl);

    student_sort_by_name(tbl);

    printf("\n=== Sorted by Name ===\n");
    student_display_all(tbl);

    printf("\nStudent Count: %u\n", student_count(tbl));

    student_table_destroy(tbl);

    return 0;
}