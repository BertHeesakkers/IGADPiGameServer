#pragma once

template <class OTHER_TYPE> 
struct AutoPointerRef
{
	explicit AutoPointerRef(OTHER_TYPE *a_Pointee)
	: m_Pointee(a_Pointee)
	{
	}

	OTHER_TYPE *m_Pointee;
};
 
template <class TYPE>
class AutoPointer 
{
public:
    typedef TYPE ElementType;
 
	explicit AutoPointer(TYPE *a_Pointee = 0)
	: m_Pointee(a_Pointee)
	{
	}

	AutoPointer(AutoPointer &a_Rhs)
	: m_Pointee(a_Rhs.Release())
	{
	}

	template <class OTHER_TYPE> 
	AutoPointer(AutoPointer<OTHER_TYPE> &a_Rhs)
	: m_Pointee(a_Rhs.Release())
	{
	}
 
	template<class OTHER_TYPE>
	operator AutoPointerRef<OTHER_TYPE>()
	{
		OTHER_TYPE *pointee = m_Pointee;
		AutoPointerRef<OTHER_TYPE> newAutoPointer(pointee);
		m_Pointee = 0;
		return newAutoPointer;
	}

	AutoPointer& operator=(AutoPointer &a_Rhs)
	{
		m_Pointee = a_Rhs.Release();
		return *this;
	}

	template <class OTHER_TYPE> 
	AutoPointer& operator=(AutoPointer<OTHER_TYPE> &a_Rhs)
	{
		m_Pointee = a_Rhs.Release();
		return *this;
	}

	AutoPointer& operator=(AutoPointerRef<TYPE> a_Rhs)
	{
		TYPE *pointee = a_Rhs.m_Pointee;
		m_Pointee = pointee;
		a_Rhs.m_Pointee = 0;
		return *this;
	}
 
    ~AutoPointer()
	{
		delete m_Pointee;
	}
 
    TYPE& operator*() const
	{
		return *m_Pointee;
	}

    TYPE* operator->() const
	{
		return m_Pointee;
	}

    TYPE* Get() const
	{
		return m_Pointee;
	}

    TYPE* Release()
	{
		TYPE *pointee = m_Pointee;
		m_Pointee = 0;
		return pointee;
	}

    void Reset(TYPE *a_Pointee = 0)
	{
		delete m_Pointee;
		m_Pointee = a_Pointee;
	}
 
	AutoPointer(AutoPointerRef<TYPE> a_Rhs)
	{
		ElementType *pointee = a_Rhs.m_Pointee;
		a_Rhs.m_Pointee = 0;
		m_Pointee = pointee;
	}
// 
// 	template <class OTHER_TYPE> operator AutoPointer<OTHER_TYPE>()
// 	{
// 		PROTO_NOT_IMPLEMENTED();
// 	}

private:
	TYPE *m_Pointee;
};

template<typename TYPE>
bool IsValid(AutoPointer<TYPE> &a_Pointer)
{
	return 0 != a_Pointer.Get();
}

template<typename TYPE>
bool IsValid(const AutoPointer<TYPE> &a_Pointer)
{
	return 0 != a_Pointer.Get();
}
