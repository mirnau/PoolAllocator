#pragma once

template<size_t elementSize, class T>
class DynamicAllocator
{
public:
	DynamicAllocator() = delete;
	DynamicAllocator(size_t capacity);
	DynamicAllocator(DynamicAllocator& other) = delete;
	DynamicAllocator(DynamicAllocator&& other) = delete;
	~DynamicAllocator();

	template<typename E, typename... Etypes>
	T* create(Etypes... args)
	{

	};

	size_t count_free();
	void dump_pool();
	void destroy(T* ptr);

	size_t m_captacity;

private:
	struct Node
	{
		T* ptr;
		Node* nextNode;
	};
	
	Node* m_head;
};