#include "test_harness.h"

int main(void){
    ASSERT(1 == 1);
    ASSERT_EQ(5, 5);
    
    return test_summary();
}