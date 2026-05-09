#include "my_vector_tests.h"
#include "my_vector.h"

void test_reallocation()
{
    log("test_reallocation");

    my_vector<int> v;

    for (int i = 0; i < 1000; ++i)
    {
        v.push_back(i);
    }

    assert(v.size() == 1000);

    for (int i = 0; i < 1000; ++i)
    {
        assert(v[i] == i);
    }
}

void test_push_back()
{
    log("test_push_back");

    my_vector<int> v;

    assert(v.size() == 0);
    assert(v.empty());

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    assert(v.size() == 3);

    assert(v[0] == 10);
    assert(v[1] == 20);
    assert(v[2] == 30);
}

void test_copy_constructor()
{
    log("test_copy_constructor");

    my_vector<int> a;

    a.push_back(5);
    a.push_back(10);

    my_vector<int> b = a;

    assert(b.size() == 2);

    assert(b[0] == 5);
    assert(b[1] == 10);

    b[0] = 100;

    assert(a[0] == 5);
}


void test_move_constructor()
{
    log("test_move_constructor");

    my_vector<int> a;

    a.push_back(1);
    a.push_back(2);

    my_vector<int> b = std::move(a);

    assert(b.size() == 2);
    assert(b[0] == 1);
    assert(b[1] == 2);

    assert(a.size() == 0);
}

void test_pop_back()
{
    log("test_move_constructor");
    
    my_vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.pop_back();

    assert(v.size() == 2);

    assert(v[0] == 1);
    assert(v[1] == 2);
}