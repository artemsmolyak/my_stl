#include "my_vector_tests.h"
#include "my_vector.h"

void test_reallocation()
{
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
    my_vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.pop_back();

    assert(v.size() == 2);

    assert(v[0] == 1);
    assert(v[1] == 2);
}

void test_copy_assignment()
{
    my_vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);

    my_vector<int> v2;
    v2.push_back(10);
    v2.push_back(20);
    v2.push_back(30);

    v1 = v2;

    assert(v1.size() == v2.size());
    for (size_t i = 0; i < v1.size(); ++i)
        assert(v1[i] == v2[i]);
}

void test_move_assignment()
{
    my_vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);

    my_vector<int> v2;
    v2.push_back(10);
    v2.push_back(20);

    v1 = std::move(v2);

    assert(v1.size() == 2);
    assert(v1[0] == 10);
    assert(v1[1] == 20);
  
    assert(v2.size() == 0 || v2.capacity() >= 0);
}

void test_strong_exception_safety()
{
    ThrowOnCopy::constructions = 0;
    ThrowOnCopy::destructions = 0;

    ThrowOnCopy::copy_count = 0;
    ThrowOnCopy::fail_after = 1000;

    {
        my_vector<ThrowOnCopy> v;
        v.reserve(3);

        v.push_back(ThrowOnCopy(1));
        v.push_back(ThrowOnCopy(2));
        v.push_back(ThrowOnCopy(3));

        my_vector<ThrowOnCopy> snapshot = v;

        ThrowOnCopy::copy_count = 0;
        ThrowOnCopy::fail_after = 0;

        bool threw = false;

        try
        {
            v.push_back(ThrowOnCopy(4));
        }
        catch (...)
        {
            threw = true;
        }

        assert(threw);

        assert(v.size() == snapshot.size());

        for (size_t i = 0; i < v.size(); ++i)
            assert(v[i].value == snapshot[i].value);
    }
    
    assert(ThrowOnCopy::constructions == ThrowOnCopy::destructions);    
}

//self assigment problem
void test_push_back_self_reference_realloc_strong()
{
    my_vector<int> v;
 
    v.reserve(1);

   v.push_back(10);
   v.push_back(20); // realloc

    int expected_first = v[0];

    v.push_back(v[0]); 

    assert(v.size() == 3);
    assert(v[0] == expected_first);
    assert(v[2] == expected_first);
}

void test_copy_constructor_exception_safety()
{
    ThrowOnCopy::constructions = 0;
    ThrowOnCopy::destructions = 0;

    ThrowOnCopy::copy_count = 0;
    ThrowOnCopy::fail_after = 4;

    bool exception_thrown = false;

    try
    {
        my_vector<ThrowOnCopy> v;

        v.push_back(ThrowOnCopy(1));
        v.push_back(ThrowOnCopy(2));
        v.push_back(ThrowOnCopy(3));

        my_vector<ThrowOnCopy> copy(v); // exception
    }
    catch (...)
    {
        exception_thrown = true;
    }

    assert(exception_thrown);
    assert(ThrowOnCopy::constructions == ThrowOnCopy::destructions);
}