
#include <iostream>
#include <new> 
#include "SharedPtr.h"
#include "ControlBlock.h"
#include "WeakPtr.h"
#include <cassert>

// Testing struct
struct Cat
{
    inline static int destructionCount = 0;
    Cat()
    {
        std::cout << "Cat created\n";
    }
   
    ~Cat()
    {
        std::cout << "Cat destroyed\n";
        ++destructionCount;
    }

    void meow()
    {
        std::cout << "Meow!\n";
    }
};

int main()
{
    // Testing copy constructor
    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat1(new Cat());

        assert(cat1.use_count() == 1);

        SharedPtr<Cat> cat2 = cat1;

        assert(cat1.use_count() == 2);
        assert(cat2.use_count() == 2);
    }
    // Checks that the Cat was destroyed exactly once
    assert(Cat::destructionCount == 1);


    //Test move construction
    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat1(new Cat());
        SharedPtr<Cat> cat2(std::move(cat1));

        assert(cat1.use_count() == 0);
        assert(cat2.use_count() == 1);
    }

    // Checks that the Cat was destroyed exactly once
    assert(Cat::destructionCount == 1);
    
    //Test copy assignment
    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat1(new Cat());
        SharedPtr<Cat> cat2(new Cat());

        cat2 = cat1;
        assert(cat1.use_count() == 2);
        assert(cat2.use_count() == 2);
        assert(Cat::destructionCount == 1);     
    }
    assert(Cat::destructionCount == 2);

    // Test Move Assignment
    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat1(new Cat());
        SharedPtr<Cat> cat2(new Cat());

        cat2 = std::move(cat1);

        assert(cat1.use_count() == 0);
        assert(cat2.use_count() == 1);
        assert(Cat::destructionCount == 1);
    }

    //Test self-assignment

    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat(new Cat());

        cat = cat;
        cat = std::move(cat);

        assert(cat.use_count() == 1);
        assert(Cat::destructionCount == 0);

    }

    assert(Cat::destructionCount == 1);

    //Testing reset()
    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat1(new Cat());
        SharedPtr<Cat> cat2 = cat1;

        cat1.reset();

        assert(cat1.use_count() == 0);
        assert(cat2.use_count() == 1);
        assert(Cat::destructionCount == 0);
    }

    assert(Cat::destructionCount == 1);

    //Testing WeakPtr

    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat(new Cat());
        WeakPtr<Cat> observer(cat);

        assert(cat.use_count() == 1);
        assert(Cat::destructionCount == 0);
        assert(!observer.expired());

        cat.reset();
        assert(cat.use_count() == 0);
        assert(Cat::destructionCount == 1);
        assert(observer.expired());
    }

    assert(Cat::destructionCount == 1);

    //Testing lock()
    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat(new Cat());
        WeakPtr<Cat> observer(cat);

        SharedPtr<Cat> lockedCat = observer.lock();

        assert(cat.use_count() == 2);
        assert(lockedCat.use_count() == 2);
        assert(Cat::destructionCount == 0);

        cat.reset();

        assert(cat.use_count() == 0);
        assert(lockedCat.use_count() == 1);
        assert(Cat::destructionCount == 0);

        lockedCat.reset();
        assert(lockedCat.use_count() == 0);
        assert(Cat::destructionCount == 1);
        assert(observer.expired());

        SharedPtr<Cat> failedLock = observer.lock();
        assert(failedLock.use_count() == 0);
        assert(failedLock.get() == nullptr);
    }
    assert(Cat::destructionCount == 1);

    //Testing WeakPtr copy and move construction

    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat(new Cat());

        WeakPtr<Cat> weak1(cat);
        WeakPtr<Cat> weak2(weak1);
        WeakPtr<Cat> weak3(std::move(weak2));

        assert(cat.use_count() == 1);
        assert(!weak1.expired());
        assert(weak2.use_count() == 0);
        assert(!weak3.expired());
    }

    assert(Cat::destructionCount == 1);

    //Testing WeakPtr copy assignment

    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat(new Cat());

        WeakPtr<Cat> weak1(cat);
        WeakPtr<Cat> weak2;

        weak2 = weak1;

        assert(cat.use_count() == 1);
        assert(!weak1.expired());
        assert(!weak2.expired());
        assert(Cat::destructionCount == 0);
        weak1.reset();
        assert(!weak2.expired());

        cat.reset();

        assert(weak2.expired());
        assert(Cat::destructionCount == 1);

        weak2.reset();

        assert(Cat::destructionCount == 1);
    }

    assert(Cat::destructionCount == 1);

    // Test WeakPtr move assignment
    {
        Cat::destructionCount = 0;

        SharedPtr<Cat> cat(new Cat());

        WeakPtr<Cat> weak1(cat);
        WeakPtr<Cat> weak2;

        weak2 = std::move(weak1);

        assert(weak1.use_count() == 0);
        assert(!weak2.expired());
        assert(cat.use_count() == 1);
        assert(Cat::destructionCount == 0);
    }

    assert(Cat::destructionCount == 1);
   

    return 0;

    }
  