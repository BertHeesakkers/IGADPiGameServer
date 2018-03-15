#pragma once

#include <map>
#include <vector>
#include <list>
#include <stdint.h>

template<typename TYPE>
void ClearContainerContents(std::vector<TYPE> &a_Container)
{
    for (typename std::vector<TYPE>::const_iterator pos = a_Container.begin(); pos != a_Container.end(); ++pos)
    {
        delete *pos;
    }
    a_Container.clear();
}

template<typename TYPE>
void ClearContainerContents(std::list<TYPE> &a_Container)
{
    for (typename std::list<TYPE>::iterator pos = a_Container.begin(); pos != a_Container.end(); ++pos)
    {
        delete *pos;
    }
    a_Container.clear();
}

// clear contents from map container
template<typename TYPE>
void ClearContainerContents(TYPE &a_Container)
{
    for (typename TYPE::iterator pos = a_Container.begin(); pos != a_Container.end(); ++pos)
    {
        delete pos->second;
    }
	a_Container.clear();
}

template<typename TYPE>
void ClearContainerContents(TYPE **a_Container, uint32_t a_Capacity)
{
	for (uint32_t index = 0; index < a_Capacity; ++index)
	{
		delete a_Container[index];
	}
}
