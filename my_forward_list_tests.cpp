
#include "my_forward_list_tests.h"


namespace my_forward_list_tests
{

void test_empty()
{
    my_forward_list<int> fl;

    assert(fl.empty());
    fl.push_front(10);
    assert(!fl.empty());

    fl.pop_front();
    assert(fl.empty());
}


}