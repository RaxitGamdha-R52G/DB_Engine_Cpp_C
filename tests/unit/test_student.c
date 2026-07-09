#include <string.h>

#include "test_framework.h"
#include "test_student.h"
#include "student_table.h"

int test_insert_and_find(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S001",
        "Alice",
        "alice@test.com",
        20,
        3.8f,
        true);

    int result =
        student_insert(tbl, &s) == STUDENT_OK &&
        student_find_by_id(tbl, "S001") != NULL;

    student_table_destroy(tbl);

    return result;
}

int test_duplicate_insert(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S001",
        "Alice",
        "alice@test.com",
        20,
        3.8f,
        true);

    StudentStatus r1 = student_insert(tbl, &s);
    StudentStatus r2 = student_insert(tbl, &s);

    student_table_destroy(tbl);

    return r1 == STUDENT_OK &&
           r2 == STUDENT_ERR_ALREADY_EXISTS;
}

int test_table_full(void)
{
    StudentTable *tbl = student_table_create();

    for (int i = 0; i < MAX_RECORDS; i++)
    {
        char id[STUDENT_ID_LEN];
        char name[STUDENT_NAME_LEN];
        char email[STUDENT_EMAIL_LEN];

        snprintf(id, sizeof(id), "S%03d", i);
        snprintf(name, sizeof(name), "Student%d", i);
        snprintf(email, sizeof(email), "s%d@test.com", i);

        Student s = make_student(id, name, email, 20, 3.5f, true);

        if (student_insert(tbl, &s) != STUDENT_OK)
        {
            student_table_destroy(tbl);
            return 0;
        }
    }

    Student extra = make_student(
        "FULL",
        "Extra",
        "extra@test.com",
        20,
        3.0f,
        true);

    StudentStatus status = student_insert(tbl, &extra);

    student_table_destroy(tbl);

    return status == STUDENT_ERR_TABLE_FULL;
}

int test_find_by_name(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S002",
        "Bob",
        "bob@test.com",
        21,
        3.5f,
        true);

    student_insert(tbl, &s);

    const Student *found =
        student_find_by_name(tbl, "Bob");

    int result =
        found != NULL &&
        strcmp(found->student_id, "S002") == 0;

    student_table_destroy(tbl);

    return result;
}

int test_delete_student(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S003",
        "Charlie",
        "charlie@test.com",
        22,
        3.2f,
        true);

    student_insert(tbl, &s);

    StudentStatus status =
        student_delete(tbl, "S003");

    int result =
        status == STUDENT_OK &&
        student_find_by_id(tbl, "S003") == NULL;

    student_table_destroy(tbl);

    return result;
}

int test_delete_not_found(void)
{
    StudentTable *tbl = student_table_create();

    StudentStatus status =
        student_delete(tbl, "UNKNOWN");

    student_table_destroy(tbl);

    return status == STUDENT_ERR_TABLE_EMPTY;
}

int test_update_student(void)
{
    StudentTable *tbl = student_table_create();

    Student old_data = make_student(
        "S004",
        "David",
        "david@test.com",
        20,
        3.0f,
        true);

    student_insert(tbl, &old_data);

    Student new_data = make_student(
        "S004",
        "David Updated",
        "updated@test.com",
        21,
        3.9f,
        true);

    StudentStatus status =
        student_update(tbl, "S004", &new_data);

    const Student *found =
        student_find_by_id(tbl, "S004");

    int result =
        status == STUDENT_OK &&
        found != NULL &&
        strcmp(found->name, "David Updated") == 0;

    student_table_destroy(tbl);

    return result;
}

int test_update_not_found(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S999",
        "Nobody",
        "nobody@test.com",
        30,
        2.0f,
        true);

    StudentStatus status =
        student_update(tbl, "S999", &s);

    student_table_destroy(tbl);

    return status == STUDENT_ERR_NOT_FOUND;
}

int test_student_count(void)
{
    StudentTable *tbl = student_table_create();

    Student s1 = make_student(
        "S001",
        "Alice",
        "alice@test.com",
        20,
        3.8f,
        true);

    Student s2 = make_student(
        "S002",
        "Bob",
        "bob@test.com",
        21,
        3.6f,
        true);

    student_insert(tbl, &s1);
    student_insert(tbl, &s2);

    int ok = (student_count(tbl) == 2);

    student_delete(tbl, "S001");

    ok &= (student_count(tbl) == 1);

    student_table_destroy(tbl);

    return ok;
}

int test_sort_by_id(void)
{
    StudentTable *tbl = student_table_create();

    Student s1 = make_student("S003", "Charlie", "c@test.com", 20, 3.0f,
                              true);
    Student s2 = make_student("S001", "Alice", "a@test.com", 20, 3.0f,
                              true);
    Student s3 = make_student("S002", "Bob", "b@test.com", 20, 3.0f,
                              true);

    student_insert(tbl, &s1);
    student_insert(tbl, &s2);
    student_insert(tbl, &s3);
    student_sort_by_id(tbl);

    const Student *s = student_find_by_id(tbl, "S001");

    int result =
        s != NULL &&
        strcmp(s->student_id, "S001") == 0;

    student_table_destroy(tbl);

    return result;
}

int test_sort_by_name(void)
{
    StudentTable *tbl = student_table_create();

    Student s1 = make_student("S003", "Charlie", "c@test.com", 20, 3.0f,
                              true);
    Student s2 = make_student("S001", "Alice", "a@test.com", 20, 3.0f,
                              true);
    Student s3 = make_student("S002", "Bob", "b@test.com", 20, 3.0f,
                              true);

    student_insert(tbl, &s1);
    student_insert(tbl, &s2);
    student_insert(tbl, &s3);
    student_sort_by_name(tbl);

    const Student *s = student_find_by_name(tbl, "Alice");

    int result =
        s != NULL &&
        strcmp(s->student_id, "S001") == 0;

    student_table_destroy(tbl);

    return result;
}

int test_insert_null_table(void)
{
    Student s = make_student(
        "S001",
        "Alice",
        "alice@test.com",
        20,
        3.8f,
        true);

    return student_insert(NULL, &s) ==
           STUDENT_ERR_INVALID_ARGUMENT;
}

int test_insert_null_student(void)
{
    StudentTable *tbl = student_table_create();

    StudentStatus status =
        student_insert(tbl, NULL);

    student_table_destroy(tbl);

    return status ==
           STUDENT_ERR_INVALID_ARGUMENT;
}

int test_update_null_arguments(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S001",
        "Alice",
        "alice@test.com",
        20,
        3.8f,
        true);

    int result =
        student_update(NULL, "S001", &s) ==
            STUDENT_ERR_INVALID_ARGUMENT &&
        student_update(tbl, NULL, &s) ==
            STUDENT_ERR_INVALID_ARGUMENT &&
        student_update(tbl, "S001", NULL) ==
            STUDENT_ERR_INVALID_ARGUMENT;

    student_table_destroy(tbl);

    return result;
}

int test_delete_null_arguments(void)
{
    StudentTable *tbl = student_table_create();

    int result =
        student_delete(NULL, "S001") ==
            STUDENT_ERR_INVALID_ARGUMENT &&
        student_delete(tbl, NULL) ==
            STUDENT_ERR_INVALID_ARGUMENT;

    student_table_destroy(tbl);

    return result;
}

int test_delete_not_found_nonempty(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S001",
        "Alice",
        "alice@test.com",
        20,
        3.8f,
        true);
    int result =
        student_insert(tbl, &s) == STUDENT_OK &&
        student_delete(tbl, "123456789") == STUDENT_ERR_NOT_FOUND;

    student_table_destroy(tbl);

    return result;
}

int test_create_table(void)
{
    StudentTable *tbl = student_table_create();

    int result = (tbl != NULL);

    student_table_destroy(tbl);

    return result;
}

int test_insert_invalid_student(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "",
        "Alice",
        "alice@test.com",
        20,
        3.5f,
        true);

    StudentStatus status = student_insert(tbl, &s);

    student_table_destroy(tbl);

    return status == STUDENT_ERR_VALIDATE;
}

int test_update_invalid_student(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S001",
        "Alice",
        "alice@test.com",
        20,
        3.5f,
        true);

    student_insert(tbl, &s);

    Student bad = make_student(
        "S001",
        "",
        "alice@test.com",
        20,
        3.5f,
        true);

    StudentStatus status =
        student_update(tbl, "S001", &bad);

    student_table_destroy(tbl);

    return status == STUDENT_ERR_VALIDATE;
}

int test_find_by_id_null_arguments(void)
{
    StudentTable *tbl = student_table_create();

    int result =
        student_find_by_id(NULL, "S001") == NULL &&
        student_find_by_id(tbl, NULL) == NULL;

    student_table_destroy(tbl);

    return result;
}

int test_find_by_name_null_arguments(void)
{
    StudentTable *tbl = student_table_create();

    int result =
        student_find_by_name(NULL, "Alice") == NULL &&
        student_find_by_name(tbl, NULL) == NULL;

    student_table_destroy(tbl);

    return result;
}

int test_find_by_id_not_found(void)
{
    StudentTable *tbl = student_table_create();

    int result =
        student_find_by_id(tbl, "UNKNOWN") == NULL;

    student_table_destroy(tbl);

    return result;
}

int test_find_by_name_not_found(void)
{
    StudentTable *tbl = student_table_create();

    int result =
        student_find_by_name(tbl, "Nobody") == NULL;

    student_table_destroy(tbl);

    return result;
}

int test_student_count_null(void)
{
    return student_count(NULL) == 0;
}

int test_destroy_null(void)
{
    student_table_destroy(NULL);
    return 1;
}

int test_sort_null(void)
{
    student_sort_by_id(NULL);
    student_sort_by_name(NULL);

    return 1;
}

int test_delete_twice(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S001",
        "Alice",
        "alice@test.com",
        20,
        3.5f,
        true);

    student_insert(tbl, &s);

    int result =
        student_delete(tbl, "S001") == STUDENT_OK &&
        student_delete(tbl, "S001") == STUDENT_ERR_TABLE_EMPTY;

    student_table_destroy(tbl);

    return result;
}

static int foreach_count = 0;

static void count_callback(const Student *student, void *userdata)
{
    (void)student;
    (void)userdata;

    foreach_count++;
}

static bool gpa_above(const Student *student, void *ctx)
{
    float min = *(float *)ctx;
    return student->gpa >= min;
}

static bool has_student_id(const Student *student, void *ctx)
{
    const char *id = ctx;
    return strcmp(student->student_id, id) == 0;
}

static int compare_by_gpa(const void *a, const void *b)
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

int test_student_foreach(void)
{
    StudentTable *tbl = student_table_create();

    foreach_count = 0;

    Student s1 = make_student("S001", "Alice", "a@test.com", 20, 3.0f, true);
    Student s2 = make_student("S002", "Bob", "b@test.com", 21, 3.5f, true);

    student_insert(tbl, &s1);
    student_insert(tbl, &s2);

    student_foreach(tbl,
                    count_callback,
                    NULL);

    student_table_destroy(tbl);

    return foreach_count == 2;
}

int test_student_filter(void)
{
    StudentTable *tbl = student_table_create();

    Student s1 = make_student("S001", "Alice", "a@test.com", 20, 3.9f, true);
    Student s2 = make_student("S002", "Bob", "b@test.com", 20, 2.5f, true);
    Student s3 = make_student("S003", "Charlie", "c@test.com", 20, 3.7f, true);

    student_insert(tbl, &s1);
    student_insert(tbl, &s2);
    student_insert(tbl, &s3);

    float min = 3.5f;

    const Student *result[MAX_RECORDS];

    uint32_t count =
        student_filter(tbl,
                       gpa_above,
                       &min,
                       result,
                       MAX_RECORDS);

    student_table_destroy(tbl);

    return count == 2;
}

int test_student_find_if(void)
{
    StudentTable *tbl = student_table_create();

    Student s = make_student(
        "S123",
        "Alice",
        "a@test.com",
        20,
        3.5f,
        true);

    student_insert(tbl, &s);

    const Student *found =
        student_find_if(tbl,
                        has_student_id,
                        "S123");

    int result =
        found != NULL &&
        strcmp(found->student_id, "S123") == 0;

    student_table_destroy(tbl);

    return result;
}

int test_student_sort_custom(void)
{
    StudentTable *tbl = student_table_create();

    Student s1 = make_student("S001", "A", "a@test.com", 20, 2.0f, true);
    Student s2 = make_student("S002", "B", "b@test.com", 20, 4.0f, true);
    Student s3 = make_student("S003", "C", "c@test.com", 20, 3.0f, true);

    student_insert(tbl, &s1);
    student_insert(tbl, &s2);
    student_insert(tbl, &s3);

    student_sort(tbl,
                 compare_by_gpa);

    const Student *top =
        student_find_by_id(tbl,
                           "S002");

    int result =
        top != NULL &&
        top->gpa == 4.0f;

    student_table_destroy(tbl);

    return result;
}

int test_student_status_str(void)
{
    return strcmp(student_status_str(STUDENT_OK),
                  "SUCCESS") == 0 &&
           strcmp(student_status_str(STUDENT_ERR_NOT_FOUND),
                  "NOT FOUND") == 0;
}

int test_student_serialize(void)
{
    Student s = make_student(
        "S001",
        "Alice",
        "a@test.com",
        20,
        3.8f,
        true);

    uint8_t buffer[sizeof(Student)];

    return student_serialize(&s,
                             buffer,
                             sizeof(buffer)) == sizeof(Student);
}

int test_student_deserialize(void)
{
    Student s1 = make_student(
        "S001",
        "Alice",
        "a@test.com",
        20,
        3.8f,
        true);

    uint8_t buffer[sizeof(Student)];

    student_serialize(&s1,
                      buffer,
                      sizeof(buffer));

    Student s2;

    int result =
        student_deserialize(&s2,
                            buffer,
                            sizeof(buffer));

    return result &&
           strcmp(s1.student_id,
                  s2.student_id) == 0 &&
           strcmp(s1.name,
                  s2.name) == 0;
}

int test_student_serialize_null(void)
{
    uint8_t buffer[sizeof(Student)];

    Student s = make_student(
        "S001",
        "Alice",
        "a@test.com",
        20,
        3.8f,
        true);

    return student_serialize(NULL,
                             buffer,
                             sizeof(buffer)) == 0 &&
           student_serialize(&s,
                             NULL,
                             sizeof(buffer)) == 0;
}

int test_student_deserialize_null(void)
{
    uint8_t buffer[sizeof(Student)];

    Student s;

    return !student_deserialize(NULL,
                                buffer,
                                sizeof(buffer)) &&
           !student_deserialize(&s,
                                NULL,
                                sizeof(buffer));
}

int main(void)
{
    /* Table lifecycle */
    RUN_TEST(test_create_table);
    RUN_TEST(test_destroy_null);

    /* Insert */
    RUN_TEST(test_insert_and_find);
    RUN_TEST(test_duplicate_insert);
    RUN_TEST(test_table_full);
    RUN_TEST(test_insert_null_table);
    RUN_TEST(test_insert_null_student);
    RUN_TEST(test_insert_invalid_student);

    /* Delete */
    RUN_TEST(test_delete_student);
    RUN_TEST(test_delete_not_found);
    RUN_TEST(test_delete_not_found_nonempty);
    RUN_TEST(test_delete_null_arguments);
    RUN_TEST(test_delete_twice);

    /* Update */
    RUN_TEST(test_update_student);
    RUN_TEST(test_update_not_found);
    RUN_TEST(test_update_null_arguments);
    RUN_TEST(test_update_invalid_student);

    /* Find */
    RUN_TEST(test_find_by_name);
    RUN_TEST(test_find_by_id_not_found);
    RUN_TEST(test_find_by_name_not_found);
    RUN_TEST(test_find_by_id_null_arguments);
    RUN_TEST(test_find_by_name_null_arguments);

    /* Count */
    RUN_TEST(test_student_count);
    RUN_TEST(test_student_count_null);

    /* Sort */
    RUN_TEST(test_sort_by_id);
    RUN_TEST(test_sort_by_name);
    RUN_TEST(test_sort_null);

    /* Generic */
    RUN_TEST(test_student_foreach);
    RUN_TEST(test_student_filter);
    RUN_TEST(test_student_find_if);
    RUN_TEST(test_student_sort_custom);

    /* Status */
    RUN_TEST(test_student_status_str);

    /* Serialization */
    RUN_TEST(test_student_serialize);
    RUN_TEST(test_student_deserialize);
    RUN_TEST(test_student_serialize_null);
    RUN_TEST(test_student_deserialize_null);

    return test_summary();
}