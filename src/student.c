#include <stdio.h>

#include "student.h"
#include <string.h>

void student_display(const Student *s)
{
    if (s == NULL)
    {
        printf("Student : NULL\n");
        return;
    }

    printf("Student Information\n");
    printf("----------------------------\n");
    printf("ID      : %s\n", s->student_id);
    printf("Name    : %s\n", s->name);
    printf("Email   : %s\n", s->email);
    printf("Age     : %u\n", s->age);
    printf("GPA     : %.2f\n", s->gpa);
    printf("Active  : %s\n", s->is_active ? "Yes" : "No");
    printf("----------------------------\n");
}

bool student_validate(const Student *student)
{
    return student != NULL &&
           student->student_id[0] != '\0' &&
           student->name[0] != '\0' &&
           student->email[0] != '\0' &&
           strchr(student->email, '@') != NULL &&
           student->age >= 1 &&
           student->age <= 120 &&
           student->gpa >= 0.0f &&
           student->gpa <= 10.0f;
}

Student make_student(
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

size_t student_serialize(const Student *student,
                         uint8_t *buf,
                         size_t buf_len)
{
    if (!student || !buf)
        return 0;

    if (buf_len < sizeof(Student))
        return 0;

    memcpy(buf, student, sizeof(Student));

    return sizeof(Student);
}

bool student_deserialize(Student *student,
                         const uint8_t *buf,
                         size_t len)
{
    if (!student || !buf)
        return false;

    if (len < sizeof(Student))
        return false;

    memcpy(student, buf, sizeof(Student));

    return true;
}
