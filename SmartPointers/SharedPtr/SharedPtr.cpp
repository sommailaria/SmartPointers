
#include <iostream>
#include <new> 

template <typename T>
class SharedPtr
{
private:
    T* data;
    int* referenceCount;

public:
    // Constructor
    SharedPtr(T* resource)
    {
        if (resource != nullptr)
        {
            data = resource;

            try
            {
                referenceCount = new int(1);
            }
            catch (const std::bad_alloc& error)
            {
                delete resource;
                throw;
            }
        }
        else
        {
            data = nullptr;
            referenceCount = nullptr;
        }
    }

    // Copy constructor
    SharedPtr(const SharedPtr& other)
    {
        data = other.data;
        referenceCount = other.referenceCount;

        if (referenceCount != nullptr)
        {
            ++(*referenceCount);
        }
    }

    // Move constructor
    SharedPtr(SharedPtr&& other)
    {
        data = other.data;
        referenceCount = other.referenceCount;

        other.data = nullptr;
        other.referenceCount = nullptr;
    }

    // Destructor
    ~SharedPtr()
    {
        if (referenceCount == nullptr)
        {
            return;
        }

        --(*referenceCount);

        if (*referenceCount == 0)
        {
            delete data;
            delete referenceCount;
        }
    }

    // Copy assignment operator
    SharedPtr& operator=(const SharedPtr& other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (referenceCount != nullptr)
        {
            --(*referenceCount);

            if (*referenceCount == 0)
            {
                delete data;
                delete referenceCount;
            }
        }

        data = other.data;
        referenceCount = other.referenceCount;

        if (referenceCount != nullptr)
        {
            ++(*referenceCount);
        }

        return *this;
    }

    // Move assignment operator
    SharedPtr& operator=(SharedPtr&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (referenceCount != nullptr)
        {
            --(*referenceCount);

            if (*referenceCount == 0)
            {
                delete data;
                delete referenceCount;
            }
        }

        data = other.data;
        referenceCount = other.referenceCount;

        other.data = nullptr;
        other.referenceCount = nullptr;

        return *this;
    }

    // Get the raw pointer
    T* get() const
    {
        return data;
    }

    // Get the number of owners
    int use_count() const
    {
        if (referenceCount == nullptr)
        {
            return 0;
        }

        return *referenceCount;
    }

    // Dereference operator
    T& operator*()
    {
        return *data;
    }

    // Member access operator
    T* operator->()
    {
        return data;
    }

    // Reset the managed resource
    void reset(T* newResource = nullptr)
    {
        if (data == newResource)
        {
            return;
        }

        int* newReferenceCount = nullptr;

        if (newResource != nullptr)
        {
            try
            {
                newReferenceCount = new int(1);
            }
            catch (const std::bad_alloc& error)
            {
                delete newResource;
                throw;
            }
        }

        // Release existing ownership
        if (referenceCount != nullptr)
        {
            --(*referenceCount);

            if (*referenceCount == 0)
            {
                delete data;
                delete referenceCount;
            }
        }

        // Adopt the new resource
        data = newResource;
        referenceCount = newReferenceCount;
    }
};

// Testing struct

struct Cat
{
    Cat()
    {
        std::cout << "Cat created\n";
    }

    ~Cat()
    {
        std::cout << "Cat destroyed\n";
    }

    void meow()
    {
        std::cout << "Meow!\n";
    }
};

int main()
{
   SharedPtr<Cat> cat1(new Cat());

    {
        SharedPtr<Cat> cat2 = cat1;

        std::cout << "Count: "
            << cat1.use_count() << '\n';
    }

    std::cout << "Count: "
        << cat1.use_count() << '\n';

    // Testing move()
    SharedPtr<Cat> cat1(new Cat());

    std::cout << "Before move:\n";
    std::cout << "cat1 count: " << cat1.use_count() << '\n';

    SharedPtr<Cat> cat2(std::move(cat1));

    std::cout << "After move:\n";
    std::cout << "cat1 count: " << cat1.use_count() << '\n';
    std::cout << "cat2 count: " << cat2.use_count() << '\n';

    cat2->meow(); 

    // Testing move assignment 

    SharedPtr<Cat> cat1(new Cat());
    SharedPtr<Cat> cat2(new Cat());

    std::cout << "Before move assignment:\n";
    std::cout << "cat1 count: " << cat1.use_count() << '\n';
    std::cout << "cat2 count: " << cat2.use_count() << '\n';

    cat2 = std::move(cat1);

    std::cout << "After move assignment:\n";
    std::cout << "cat1 count: " << cat1.use_count() << '\n';
    std::cout << "cat2 count: " << cat2.use_count() << '\n';

    cat2->meow(); 

    // Testing self assignment 
    SharedPtr<Cat> cat1(new Cat());

    std::cout << "Initial count: "
        << cat1.use_count() << '\n';

    cat1 = cat1;

    std::cout << "After copy self-assignment: "
        << cat1.use_count() << '\n';

    cat1 = std::move(cat1);

    std::cout << "After move self-assignment: "
        << cat1.use_count() << '\n';

    cat1->meow(); 

    SharedPtr<Cat> cat1(nullptr);

    std::cout << "cat1 count: "
        << cat1.use_count() << '\n';

    SharedPtr<Cat> cat2 = cat1;

    std::cout << "cat2 count: "
        << cat2.use_count() << '\n';

    cat1.reset(new Cat());

    std::cout << "After cat1 reset:\n";
    std::cout << "cat1 count: "
        << cat1.use_count() << '\n';
    std::cout << "cat2 count: "
        << cat2.use_count() << '\n';

    SharedPtr<Cat> cat3 = cat1;

    cat1.reset();

    std::cout << "After second reset:\n";
    std::cout << "cat1 count: "
        << cat1.use_count() << '\n';
    std::cout << "cat3 count: "
        << cat3.use_count() << '\n';

    return 0;
}