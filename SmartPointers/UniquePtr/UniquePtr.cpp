#include <iostream>
#include <utility>


template <typename T>
class UniquePtr
{
private:
    T* data = nullptr;
public:
    // Constructors: 
    UniquePtr() = default;

    explicit UniquePtr(T* resource)
    {
        data = resource;
    }

    //Destructor
    ~UniquePtr()
    {
        delete data;
    }

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    //Move Constructor

    UniquePtr(UniquePtr&& other)
    {
        data = other.data;
        other.data = nullptr;
    }

    //Move assignment
    UniquePtr& operator=(UniquePtr&& other)
    {
        if (&other == this)
        {
            return *this;
        }
        delete data;
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    // Get() to return raw pointer 
    T* get() const
    {
        return data;
    }

    //Release() to release ownership of the pointer

    T* release()
    {
        T* resourceAddress = data;
        data = nullptr;
        return resourceAddress;
    }

    //Reset function to replace the object we are managing with another one 

    void reset(T* newResource = nullptr)
    {
        if (newResource == data)
        {
            return;
        }
        T* temp = data;
        data = newResource;
        delete temp;
    }

    // * operator to dereference the pointer 
    T& operator*() const
    {
        return *data;
    }

    // -> operator
    T* operator->() const
    {
        return data;
    }
};

// Testing Struct
struct Cat
{
    int age = 2;

    Cat()
    {
        std::cout << "A new cat was created!\n";
    }

    void meow()
    {
        std::cout << "Miao! I'm a cat \n";
    }

    ~Cat()
    {
        std::cout << "Arrivederci! \n";
    }
};

int main()
{
    // Testing get() and release()
    {
        UniquePtr<Cat> cat(new Cat());
        Cat* gigi = cat.get();

        gigi->meow();

        Cat* gigiTwo = cat.release();
        gigiTwo->meow();

        std::cout << "Before deleting GigiTwo\n";
        delete gigiTwo;
        std::cout << "After deleting GigiTwo\n";
    }


    // Testing reset()
    {
        std::cout << "Testing reset \n";

        UniquePtr<Cat> firstCat(new Cat());
        firstCat.reset(new Cat());

        UniquePtr<Cat> secondCat(new Cat());
        secondCat.reset();

        std::cout << "Reset completed \n";
    }


    // Testing move assignment
    {
        std::cout << "Testing move assignment \n";

        UniquePtr<Cat> originalCat(new Cat());
        UniquePtr<Cat> newCat(new Cat());

        originalCat = std::move(newCat);

        std::cout << "Move assignment completed\n";
    }


    // Testing move construction
    {
        std::cout << "Testing move construction \n";

        UniquePtr<Cat> tabbyCat(new Cat());
        Cat* originalAddress = tabbyCat.get();

        UniquePtr<Cat> newTabbyCat(std::move(tabbyCat));
        Cat* newAddress = newTabbyCat.get();

        if (tabbyCat.get() == nullptr)
        {
            std::cout << "tabbyCat is empty!\n";
        }

        std::cout << "Testing move construction done \n";

        if (originalAddress == newAddress)
        {
            std::cout << "newTabbyCat is not empty \n";
        }
    }


    // Testing self-move assignment
    {
        std::cout << "Testing self move \n";

        UniquePtr<Cat> blackCat(new Cat());
        Cat* blackCatAddress = blackCat.get();

        blackCat = std::move(blackCat);

        Cat* newBlackCatAddress = blackCat.get();

        if (blackCatAddress == newBlackCatAddress && blackCat.get() != nullptr)
        {
            std::cout << "Self-move assignment preserved ownership!\n";
        }
        else
        {
            std::cout << "Self-move assignment failed!\n";
        }

        std::cout << "Testing self move completed \n";
    }


    // Testing operator*() and operator->()
    {
        UniquePtr<Cat> lastCat(new Cat());

        std::cout << "Testing * operator \n";

        (*lastCat).age += 2;

        int catAge = (*lastCat).age;

        std::cout << lastCat->age << std::endl;
        std::cout << "The cat is now " << catAge << " years old!\n";
    }

    // Testing empty pointer 

    {
        std::cout << "Testing empty pointer \n";
        UniquePtr<Cat> emptyCat;
        if (emptyCat.get() == nullptr)
        {
            std::cout << "Smart pointer is empty! \n";
        }
        emptyCat.reset(new Cat());
        if (emptyCat.get() != nullptr)
        {
        std::cout << "Smart pointer now owns Gigi! \n";
        }

    }

    return 0;
}