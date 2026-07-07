#include <stdio.h>

#include "student_table.h"

int main(void)
{
    StudentTable *tbl = student_table_create();

    Student student1 = {
        .student_id = "923456789012345",
        .name = "Alice in Borderland",
        .email = "alice@borderland.com",
        .age = 24,
        .gpa = 8.9f,
        .is_active = true};

    Student student2 = {
        .student_id = "123456789012346",
        .name = "Borderland",
        .email = "alice@borderland.com",
        .age = 20,
        .gpa = 8.0f,
        .is_active = true};

    Student student3 = {
        .student_id = "123456789012347",
        .name = "Charlie",
        .email = "alice@borderland.com",
        .age = 25,
        .gpa = 9.4f,
        .is_active = true};

    if (student_insert(tbl, &student1) == STUDENT_OK)
        printf("Insert Successful\n");
    if (student_insert(tbl, &student2) == STUDENT_OK)
        printf("Insert Successful\n");
    if (student_insert(tbl, &student3) == STUDENT_OK)
        printf("Insert Successful\n");

    const char *id = student2.student_id;

    // const Student *s = student_find_by_id(tbl, id);

    // if (s)
    //     printf("Fetch Successful\n");

    if (student_delete(tbl, id) == STUDENT_OK)
        printf("Delete Successful\n");

    student_display_all(tbl);
    // printf("%u\n",student_count(tbl));

    student_sort_by_id(tbl);

    student_display_all(tbl);

    student_sort_by_name(tbl);

    student_display_all(tbl);

    student_table_destroy(tbl);
    tbl = NULL;

    return 0;
}