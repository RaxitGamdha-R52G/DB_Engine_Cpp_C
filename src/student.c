#include <stdio.h>

#include "student.h"
#include <string.h>

void student_display(const Student *s)
{
    if (s == NULL)
    {
        printf("Student: NULL\n");
        return;
    }

    printf("Student Information\n");
    printf("----------------------------\n");
    printf("ID        : %s\n", s->student_id);
    printf("Name      : %s\n", s->name);
    printf("Email     : %s\n", s->email);
    printf("Age       : %u\n", s->age);
    printf("GPA       : %.2f\n", s->gpa);
    printf("Active    : %s\n", s->is_active ? "Yes" : "No");
    printf("----------------------------\n");
}

bool student_validate(const Student *student)
{
    return student != NULL &&
           student->student_id[0] != '\0' &&
           student->name[0] != '\0' &&
           student->email[0] != '\0' &&
           strchr(student->email, '@') != NULL &&
           student->age >= 5 &&
           student->age <= 120 &&
           student->gpa >= 0.0f &&
           student->gpa <= 4.0f;
}
