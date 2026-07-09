#ifndef TEST_STUDENT_H
#define TEST_STUDENT_H

/* Table lifecycle */
int test_create_table(void);
int test_destroy_null(void);

/* Insert */
int test_insert_and_find(void);
int test_duplicate_insert(void);
int test_table_full(void);
int test_insert_null_table(void);
int test_insert_null_student(void);
int test_insert_invalid_student(void);

/* Delete */
int test_delete_student(void);
int test_delete_not_found(void);
int test_delete_not_found_nonempty(void);
int test_delete_null_arguments(void);
int test_delete_twice(void);

/* Update */
int test_update_student(void);
int test_update_not_found(void);
int test_update_null_arguments(void);
int test_update_invalid_student(void);

/* Find */
int test_find_by_id_not_found(void);
int test_find_by_name(void);
int test_find_by_name_not_found(void);
int test_find_by_id_null_arguments(void);
int test_find_by_name_null_arguments(void);

/* Count */
int test_student_count(void);
int test_student_count_null(void);

/* Sort */
int test_sort_by_id(void);
int test_sort_by_name(void);
int test_sort_null(void);

/* Generic Algorithms */
int test_student_foreach(void);
int test_student_filter(void);
int test_student_find_if(void);

/* Generic Sort */
int test_student_sort_custom(void);

/* Status */
int test_student_status_str(void);

/* Serialization */
int test_student_serialize(void);
int test_student_deserialize(void);
int test_student_serialize_null(void);
int test_student_deserialize_null(void);

#endif /* TEST_STUDENT_H */