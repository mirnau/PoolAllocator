#pragma once

#include<memory>
#include<iostream>

#include "dynamicAllocator.h"

template<size_t elementSize, typename T>
DynamicAllocator<elementSize, T>::DynamicAllocator(size_t capacity)
{
	size_t storageSpace = sizeof(T) * capacity;
	auto ptr = static_cast<T*>std::malloc(sizeof(T) * capacity)

}

template<size_t elementSize, typename T>
DynamicAllocator<elementSize, T>::~DynamicAllocator()
{
}

template<size_t elementSize, typename T>
size_t DynamicAllocator<elementSize, T>::count_free()
{
	return size_t();
}

template<size_t elementSize, typename T>
void DynamicAllocator<elementSize, T>::dump_pool()
{
	std::out << "Capacity: " + m_captacity << std::endl;
}

template<size_t elementSize, typename T>
inline void DynamicAllocator<elementSize, T>::destroy(T* ptr)
{
	//Calls the destructor of the object
	//Adds the element to the free-list
}
