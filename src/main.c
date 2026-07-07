#include <stdio.h>
#include <string.h>

#include "student_table.h"

static Student make_student(
    const char *id,
    const char *name,
    const char *email,
    uint32_t age,
    float gpa,
    bool is_active)
{
    Student s = {0};

    strncpy(s.student_id, id, STUDENT_ID_LEN - 1);
    strncpy(s.name, name, STUDENT_NAME_LEN - 1);
    strncpy(s.email, email, STUDENT_EMAIL_LEN - 1);

    s.age = age;
    s.gpa = gpa;
    s.is_active = is_active;

    return s;
}

static void print_status(const char *operation, StudentStatus status)
{
    printf("%-10s : ", operation);

    switch (status)
    {
    case STUDENT_OK:
        printf("SUCCESS\n");
        break;
    case STUDENT_ERR_INVALID_ARGUMENT:
        printf("INVALID ARGUMENT\n");
        break;
    case STUDENT_ERR_NOT_FOUND:
        printf("NOT FOUND\n");
        break;
    case STUDENT_ERR_ALREADY_EXISTS:
        printf("ALREADY EXISTS\n");
        break;
    case STUDENT_ERR_TABLE_FULL:
        printf("TABLE FULL\n");
        break;
    case STUDENT_ERR_TABLE_EMPTY:
        printf("TABLE EMPTY\n");
        break;
    case STUDENT_ERR_OUT_OF_MEMORY:
        printf("OUT OF MEMORY\n");
        break;
    case STUDENT_ERR_VALIDATE:
        printf("VALIDATION FAILED\n");
        break;
    default:
        printf("UNKNOWN ERROR\n");
    }
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

    print_status("Insert 1", student_insert(tbl, &student1));
    print_status("Insert 2", student_insert(tbl, &student2));
    print_status("Insert 3", student_insert(tbl, &student3));

    printf("\n=== Students ===\n");
    student_display_all(tbl);

    const Student *found =
        student_find_by_id(tbl, "123456789012346");

    if (found != NULL)
    {
        printf("\nStudent Found:\n");
        student_display(found);
    }

    print_status("Delete", student_delete(tbl, "123456789012346"));

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