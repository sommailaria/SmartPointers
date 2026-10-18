#pragma once
#include "ControlBlock.h"
#include "SharedPtr.h"

template <typename T>
class WeakPtr
{
private:
	ControlBlock<T>* controlBlock = nullptr;
public:
	//Constructor
	WeakPtr() {};

	WeakPtr(const SharedPtr<T>& other)
	{
		controlBlock = other.controlBlock;

		if (controlBlock != nullptr)
		{
			++controlBlock->weakCounter;
		}
	}

	//Destructor

	~WeakPtr()
	{
		if (controlBlock == nullptr)
		{
			return;
		}
		--controlBlock->weakCounter;

		if (controlBlock->weakCounter == 0 && controlBlock->strongCounter == 0)
		{
			delete controlBlock;
		}
	}

	//Copy Constructor
	WeakPtr(const WeakPtr& other)
	{
		controlBlock = other.controlBlock;
		if (controlBlock != nullptr)
		{
			++controlBlock->weakCounter;
		}
	}

	//Move Constructor
	WeakPtr(WeakPtr&& other)
	{
		controlBlock = other.controlBlock;
		other.controlBlock = nullptr;
	}

	//Expired()
	bool expired() const
	{
		if (controlBlock != nullptr && controlBlock->strongCounter > 0)
		{
			return false;
		}

		return true;
	}

	//Lock()
	SharedPtr<T> lock() const
	{
		if (expired() == true)
		{
			return SharedPtr<T>(nullptr);
		}
		return SharedPtr<T>(controlBlock);
	}

	//Copy assignment
	WeakPtr& operator=(const WeakPtr& other)
	{
		if (this == &other)
		{
			return *this;
		}
		
		if (controlBlock != nullptr)
		{
			--controlBlock->weakCounter;

			if (controlBlock->weakCounter == 0 && controlBlock->strongCounter == 0)
			{
				delete controlBlock;
			}
		}
		
		controlBlock = other.controlBlock;

		if (controlBlock != nullptr)
		{
			++controlBlock->weakCounter;
		}

		return *this;
	}

	//Move assignment 
	WeakPtr& operator=(WeakPtr&& other)
	{

		if (this == &other)
		{
			return *this;
		}

		if (controlBlock != nullptr)
		{
			--controlBlock->weakCounter;

			if (controlBlock->weakCounter == 0 && controlBlock->strongCounter == 0)
			{
				delete controlBlock;
			}
		}

		
		controlBlock = other.controlBlock;
		other.controlBlock = nullptr;
		return *this;
	}

	void reset()
	{

		if (controlBlock == nullptr)
		{
			return;
		}
		--controlBlock->weakCounter;

		if (controlBlock->weakCounter == 0 && controlBlock->strongCounter == 0)
		{
			delete controlBlock;
		}

		controlBlock = nullptr;
	}

	int use_count() const
	{
		if (controlBlock == nullptr)
		{
			return 0;
		}

		return controlBlock->strongCounter;

	}
};

