#pragma once

template <typename T>
struct ControlBlock
{
    T* managedObject = nullptr;
    int strongCounter;
    int weakCounter;

    ControlBlock(T* item)
    {
        managedObject = item;
        strongCounter = 1;
        weakCounter = 0;
    }
};
