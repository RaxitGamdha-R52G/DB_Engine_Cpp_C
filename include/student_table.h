#ifndef STUDENT_TABLE_H
#define STUDENT_TABLE_H

#include "student.h"

typedef enum
{
    STUDENT_OK = 0,
    STUDENT_ERR_INVALID_ARGUMENT,
    STUDENT_ERR_NOT_FOUND,
    STUDENT_ERR_ALREADY_EXISTS,
    STUDENT_ERR_TABLE_FULL,
    STUDENT_ERR_TABLE_EMPTY,
    STUDENT_ERR_OUT_OF_MEMORY
} StudentStatus;

typedef struct StudentTable StudentTable;

StudentTable *student_table_create(void);
void student_table_destroy(StudentTable *tbl);

StudentStatus student_insert(StudentTable *tbl, const Student *student);
StudentStatus student_delete(StudentTable *tbl, const char *id);
StudentStatus student_update(StudentTable *tbl, const char *id, const Student *new_data);

const Student *student_find_by_id(const StudentTable *tbl, const char *id);
const Student *student_find_by_name(const StudentTable *tbl, const char *name);

uint32_t student_count(const StudentTable *tbl);

void student_display_all(const StudentTable *tbl);
void student_sort_by_id(StudentTable *tbl);
void student_sort_by_name(StudentTable *tbl);

#endif