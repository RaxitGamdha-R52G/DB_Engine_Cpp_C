#ifndef STUDENT_TABLE_H
#define STUDENT_TABLE_H

#include "student.h"

#define MAX_RECORDS 1000

// Status Codes
typedef enum
{
    STUDENT_OK = 0,
    STUDENT_ERR_INVALID_ARGUMENT,
    STUDENT_ERR_NOT_FOUND,
    STUDENT_ERR_ALREADY_EXISTS,
    STUDENT_ERR_TABLE_FULL,
    STUDENT_ERR_TABLE_EMPTY,
    STUDENT_ERR_OUT_OF_MEMORY,
    STUDENT_ERR_VALIDATE
} StudentStatus;

// Forward Declaration
typedef struct StudentTable StudentTable;

// Generic Callback Types
typedef void (*student_foreach_fn)(const Student *student,
                                   void *userdata);

typedef bool (*student_predicate_fn)(const Student *student,
                                     void *ctx);

// Table Lifecycle
StudentTable *student_table_create(void);
void student_table_destroy(StudentTable *tbl);

// CRUD Operations
StudentStatus student_insert(StudentTable *tbl,
                             const Student *student);

StudentStatus student_delete(StudentTable *tbl,
                             const char *id);

StudentStatus student_update(StudentTable *tbl,
                             const char *id,
                             const Student *new_data);

// Search
const Student *student_find_by_id(const StudentTable *tbl,
                                  const char *id);

const Student *student_find_by_name(const StudentTable *tbl,
                                    const char *name);

// Generic Algorithms
void student_foreach(const StudentTable *tbl,
                     student_foreach_fn fn,
                     void *userdata);

uint32_t student_filter(const StudentTable *tbl,
                        student_predicate_fn predicate,
                        void *ctx,
                        const Student **out,
                        uint32_t max);

const Student *student_find_if(const StudentTable *tbl,
                               student_predicate_fn predicate,
                               const void *ctx);

void student_sort(StudentTable *tbl,
                  int (*cmp)(const void *, const void *));

// Display & Sorting
void student_display_all(const StudentTable *tbl);

void student_sort_by_id(StudentTable *tbl);

void student_sort_by_name(StudentTable *tbl);

uint32_t student_count(const StudentTable *tbl);

// Status Utilities
const char *student_status_str(StudentStatus status);

void student_print_status(const char *operation,
                          StudentStatus status);

#endif /* STUDENT_TABLE_H */