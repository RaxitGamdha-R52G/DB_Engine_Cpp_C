#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct{
    char student_id[16];
    char name[64];
    char email[128];
    uint32_t age;
    float gpa;
    bool isActive;
} Student;

void print_student_record(const Student *student);

#endif /* STUDENT_H  */