#pragma once
#include <new>
#include "ControlBlock.h"
#include <cstddef>


template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr
{
private:
    friend class WeakPtr<T>;
    ControlBlock<T>* controlBlock = nullptr;
    SharedPtr(ControlBlock<T>* existingBlock)
    {
        controlBlock = existingBlock;
        if (controlBlock != nullptr)
        {
            ++controlBlock->strongCounter;
        }
    }

public:
    // Constructor
    SharedPtr(T* resource)
    {
        if (resource != nullptr)
        {
            try
            {
                controlBlock = new ControlBlock<T>(resource);
            }
            catch (const std::bad_alloc& error)
            {
                delete resource;
                throw;
            }
        }
    }

    SharedPtr(std::nullptr_t) {}

    // Copy constructor
    SharedPtr(const SharedPtr& other)
    {
        controlBlock = other.controlBlock;

        if (controlBlock != nullptr)
        {
            ++controlBlock->strongCounter;
        }
    }

    // Move constructor
    SharedPtr(SharedPtr&& other)
    {
        controlBlock = other.controlBlock;
        other.controlBlock = nullptr;
    }

    // Destructor
    ~SharedPtr()
    {
        if (controlBlock == nullptr)
        {
            return;
        }

        --(controlBlock->strongCounter);

        if (controlBlock->strongCounter == 0)
        {
            delete controlBlock->managedObject;
            controlBlock->managedObject = nullptr;
            if (controlBlock->weakCounter == 0)
            {
                delete controlBlock;
            }

        }
    }

    // Copy assignment operator
    SharedPtr& operator=(const SharedPtr& other)
    {
        if (this == &other)
        {
            return *this;
        }
        if (controlBlock != nullptr)
        {
            --(controlBlock->strongCounter);

            if (controlBlock->strongCounter == 0)
            {
                delete controlBlock->managedObject;
                controlBlock->managedObject = nullptr;
                if (controlBlock->weakCounter == 0)
                {
                    delete controlBlock;
                }

            }
        }

        controlBlock = other.controlBlock;

        if (controlBlock != nullptr)
        {
            ++controlBlock->strongCounter;
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

        if (controlBlock != nullptr)
        {
            --(controlBlock->strongCounter);

            if (controlBlock->strongCounter == 0)
            {
                delete controlBlock->managedObject;
                controlBlock->managedObject = nullptr;
                if (controlBlock->weakCounter == 0)
                {
                    delete controlBlock;
                }

            }
        }
        controlBlock = other.controlBlock;
        other.controlBlock = nullptr;
        return *this;
    }

    // Get the raw pointer
    T* get() const
    {
        if (controlBlock == nullptr)
        {
            return nullptr;
        }

        return controlBlock->managedObject;

    }

    // Get the number of owners
    int use_count() const
    {
        if (controlBlock == nullptr)
        {
            return 0;
        }

        return controlBlock->strongCounter;
    }

    // Dereference operator
    T& operator*()
    {
        return *controlBlock->managedObject;
    }

    // Member access operator
    T* operator->()
    {
        return controlBlock->managedObject;
    }

    // Reset the managed resource
    void reset(T* newResource = nullptr)
    {
        if (get() == newResource)
        {
            return;
        }
        ControlBlock<T>* newControlBlock = nullptr;
        if (newResource != nullptr)
        {
            try
            {
                newControlBlock = new ControlBlock<T>(newResource);
            }
            catch (const std::bad_alloc& error)
            {
                delete newResource;
                throw;
            }
        }

        // Release existing ownership
        if (controlBlock != nullptr)
        {
            --(controlBlock->strongCounter);

            if (controlBlock->strongCounter == 0)
            {
                delete controlBlock->managedObject;
                controlBlock->managedObject = nullptr;
                if (controlBlock->weakCounter == 0)
                {
                    delete controlBlock;
                }

            }
        }

        // Adopt the new resource
        controlBlock = newControlBlock;
    }
};
