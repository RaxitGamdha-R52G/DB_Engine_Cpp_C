#include "student.h"

int main(void){
    const Student student = {
        .student_id = "123456789012345",
        .name = "Alice in Borderland",
        .email = "alice@borderland.com",
        .age = 22,
        .gpa = 8.5f,
        .isActive = true
    };

    print_student_record(&student);

    return 0;
}