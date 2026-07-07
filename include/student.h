#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>
#include <stdint.h>

#define STUDENT_ID_LEN 16
#define STUDENT_NAME_LEN 64
#define STUDENT_EMAIL_LEN 128

typedef struct
{
    char student_id[STUDENT_ID_LEN];
    char name[STUDENT_NAME_LEN];
    char email[STUDENT_EMAIL_LEN];
    uint32_t age;
    float gpa;
    bool is_active;
} Student;

void student_display(const Student *s);

bool student_validate(const Student *student);

#endif /* STUDENT_H  */