#include "my_unique_ptr_tests.h"
#include "my_unique_ptr.h"

namespace my_unique_ptr_tests
{

void test_basic_ownership()
{
    Counter::constructions = 0;
    Counter::destructions = 0;

    {
        my_unique_ptr<Counter> ptr(new Counter());

        assert(ptr.get() != nullptr);
        assert(Counter::constructions == 1);
        assert(Counter::destructions == 0);
    }

    assert(Counter::destructions == 1);

    std::cout << "test_basic_ownership passed\n";
}

void test_move_constructor()
{
    my_unique_ptr<int> p1(new int(42));
    my_unique_ptr<int> p2(std::move(p1));

    assert(p1.get() == nullptr);
    assert(p2.get() != nullptr);
    assert(*p2 == 42);

    std::cout << "test_move_constructor passed\n";
}

void test_move_assignment()
{
    my_unique_ptr<int> p1(new int(10));
    my_unique_ptr<int> p2(new int(20));

    p2 = std::move(p1);

    assert(p1.get() == nullptr);
    assert(p2.get() != nullptr);
    assert(*p2 == 10);

    std::cout << "test_move_assignment passed\n";
}

void test_reset()
{
    Counter::constructions = 0;
    Counter::destructions = 0;

    my_unique_ptr<Counter> p(new Counter());
    assert(Counter::constructions == 1);

    p.reset(new Counter());
    assert(Counter::constructions == 2);
    assert(Counter::destructions == 1);

    p.reset();
    assert(p.get() == nullptr);
    assert(Counter::destructions == 2);

    std::cout << "test_reset passed\n";
}

void test_release()
{
    my_unique_ptr<int> p(new int(99));

    int* raw = p.release();

    assert(p.get() == nullptr);
    assert(*raw == 99);

    delete raw;

    std::cout << "test_release passed\n";
}

} // namespace