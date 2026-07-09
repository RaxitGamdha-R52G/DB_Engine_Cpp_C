#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define STUDENT_ID_LEN    16
#define STUDENT_NAME_LEN  64
#define STUDENT_EMAIL_LEN 128

// Student Record
typedef struct
{
    char student_id[STUDENT_ID_LEN];
    char name[STUDENT_NAME_LEN];
    char email[STUDENT_EMAIL_LEN];

    uint32_t age;
    float gpa;

    bool is_active;

} Student;

// Display
void student_display(const Student *s);

// Validation
bool student_validate(const Student *student);

// Construction
Student make_student(
    const char *id,
    const char *name,
    const char *email,
    uint32_t age,
    float gpa,
    bool is_active);

// Serialization
size_t student_serialize(const Student *student,
                         uint8_t *buffer,
                         size_t buffer_len);

bool student_deserialize(Student *student,
                         const uint8_t *buffer,
                         size_t length);

#endif /* STUDENT_H */