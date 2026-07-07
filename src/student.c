#include <stdio.h>

#include "student.h"

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
