#pragma once
#include <memory>
#include <iostream>

template<size_t elementSize, class IndexType>
class DynamicAllocator
{
public:

	union FreeListNode
	{
		IndexType m_index;
		char data[elementSize];

		FreeListNode(size_t index)
		{
			m_index = index;
		}
	};

public:
	DynamicAllocator() = delete;

	DynamicAllocator(size_t capacity) :
		m_capacity(capacity),
		m_arena(allocate(capacity))
	{
		FreeListNode* m_freeList = reinterpret_cast<FreeListNode*>(m_arena);

		for (size_t i = 0; i < m_capacity - 1; i++)
			m_freeList[i] = i + 1;

		m_freeList[capacity - 1].m_index = capacity;
		m_head = 0;
	}

	//DynamicAllocator(DynamicAllocator&) = delete;
	//DynamicAllocator(DynamicAllocator&&) = delete;

	~DynamicAllocator()
	{
		std::free(m_arena);
	}
public:
	//DynamicAllocator& operator=(DynamicAllocator&) = delete;
	//DynamicAllocator& operator=(DynamicAllocator&&) = delete;

public:

	FreeListNode* allocate(std::size_t n)
	{
		if (auto ptr = static_cast<FreeListNode*>(std::malloc(n * elementSize)))
			return ptr;

		throw std::bad_alloc();
	}

	template <typename T, typename... TArgs>
	T* create(TArgs... args)
	{
		if (m_head != m_capacity && sizeof(T) <= elementSize)
		{
			IndexType index = m_arena[m_head].m_index;
			T* ptr = new(&m_arena[m_head]) T(std::forward<TArgs>(args)...);
			m_head = index;
			return ptr;
		}

		return nullptr;
	}

	size_t count_free()
	{
		size_t free = 0;

		IndexType current = m_head;

		while (m_arena[current].m_index < m_capacity)
		{
			free++;
			current = m_arena[current].m_index;

			if (m_arena[current].m_index == m_capacity)
			{
				free++; //The last element
			}
		}

		return free;
	}

	void dump_pool()
	{
		std::cout
			<< "BEGIN DUMP" << std::endl
			<< "*******************" << std::endl

			<< "Capacity: " << m_capacity << std::endl
			<< "Free Slots: " << count_free() << std::endl

			<< "*******************" << std::endl
			<< "END DUMP" << std::endl
			<< std::endl;
	}
	
	template <typename T>
	void destroy(T* ptr)
	{
		if (ptr) {
			FreeListNode* p = reinterpret_cast<FreeListNode*>(ptr);
			size_t index = p - m_arena;
			ptr->~T();
			m_arena[index].m_index = m_head;
			m_head = index;
		}
	}

private:
	FreeListNode* m_arena;
	IndexType m_head;
	size_t m_capacity;
};