#include <stdio.h>

#include "student.h"

void print_student_record(const Student *student){
    printf("ID: %s\n", student->student_id);
    printf("Name: %s\n", student->name);
    printf("Email ID: %s\n", student->email);
    printf("Age: %u\n", student->age);
    printf("GPA: %.2f\n", student->gpa);
    printf("Active: %s\n", student->isActive ? "Yes" : "No");
}
