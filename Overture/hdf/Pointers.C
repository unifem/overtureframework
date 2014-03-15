#define _POINTERS_C_ "$Id: Pointers.C,v 1.3 1999/12/11 00:21:33 henshaw Exp $"

//
// Pointers with copy symante (can get at copy constructor)
//

template<class T>
CpPtr<T>::CpPtr(const CpPtr<T>& _a)
    : ptr(_a.isNull() ?  0 : new T(*_a.ptr))
{}

template<class T>
CpPtr<T>&
CpPtr<T>::operator=(T* _ptr)
{
    delete ptr;
    ptr = _ptr;
    return *this;
}

template<class T>
CpPtr<T>&
CpPtr<T>::operator=(const CpPtr<T>& _r)
{
    if (ptr != _r.ptr) {
	delete ptr;
	ptr = _r.isNull() ? 0 : new T(*_r.ptr);
    }
    return *this;
}

template<class T>
T*
CpPtr<T>::release()
{
    T* old = ptr;
    ptr = 0;
    return old;
}


//
// Aliased Pointers
//

template<class T>
LnPtr<T>&
LnPtr<T>::operator=(const LnPtr<T>& _r)
{
    if (ptr != _r.ptr) {
	if (unique()) delete ptr;
	ptr = _r.ptr;
	ucnt = _r.ucnt;
    }
    return *this;
}

template<class T>
LnPtr<T>&
LnPtr<T>::operator=(T* _ptr)
{ 
    if (unique()) delete ptr;
    ptr = _ptr;
    ucnt = UseCount();
    return *this;
}


template<class T>
LnPtr<T>::~LnPtr()
{ 
    if (ucnt.unique()) delete ptr;
}

template<class T>
bool
LnPtr<T>::unique() const
{ 
    return ucnt.unique();
}

template<class T>
int
LnPtr<T>::linkCount() const
{ 
    return ucnt.linkCount();
}


//
// Clonable Class Pointers
//

template<class T>
VcClassPtr<T>::VcClassPtr(const VcClassPtr<T>& _a)
    :  ptr(_a.isNull() ?  0 : _a.ptr->clone())
{
}

template<class T>
VcClassPtr<T>&
VcClassPtr<T>::operator=(T* _ptr)
{
    delete ptr;
    ptr = _ptr;
    return *this;
}

template<class T>
VcClassPtr<T>&
VcClassPtr<T>::operator=(const VcClassPtr<T>& _r)
{
    if (ptr != _r.ptr) {
	delete ptr;
	ptr = _r.isNull() ? 0 : _r.ptr->clone();
    }
    return *this;
}

template<class T>
T*
VcClassPtr<T>::release()
{
    T* old = ptr;
    ptr = 0;
    return old;
}

