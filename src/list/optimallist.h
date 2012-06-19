#ifndef OPTIMALLIST_H
#define OPTIMALLIST_H

#include <QDebug>
#include <QByteArray>

template <typename T>
class OptimalList
{
public:
    OptimalList();
    ~OptimalList();

    void clear();

    void reserve(int size);

    void prepend(const T &t);
    void append(const T &t);
    void insert(int i, const T &t);

    void replace(int i, const T &t);

    void removeAt(int i);
    void removeFirst();
    void removeLast();
    int  removeAll(const T &t);
    bool removeOne(const T &t);

    T takeAt(int i);
    T takeFirst();
    T takeLast();

    const T &at(int i) const;
    const T &operator[](int i) const;
    T &operator[](int i);

    int length() const;
    int size() const;
    int count() const;

    bool isEmpty() const;

    void move(int from, int to);
    void swap(int i, int j);
    void swap(OptimalList<T> &other);

    int indexOf(const T &t, int from = 0) const;
    int lastIndexOf(const T &t, int from = -1) const;
    QBool contains(const T &t) const;

    int count(const T &t) const;

    OptimalList<T> &operator=(const OptimalList<T> &l);
    bool operator==(const OptimalList<T> &l) const;
    bool operator!=(const OptimalList<T> &l) const;

protected:
    enum
    {
        // Should be like this, but we use as below. Maybe it is good
//      isLarge       = QTypeInfo<T>::isLarge        || QTypeInfo<T>::isStatic,
        isLarge       = (sizeof(T)>sizeof(void *)*4),// || QTypeInfo<T>::isStatic,
        sizeOfElement = isLarge ? sizeof(void *) : sizeof(T)
    };

    void* mBuffer;
    int   mBegin;
    int   mCount;
    int   mCapacity;
    int   mReserved;

    void setOptimalCapacity();
    inline void* pointerAt(const int i);
};

template <typename T>
OptimalList<T>::OptimalList()
{
    mBuffer=0;
    mBegin=0;
    mCount=0;
    mCapacity=0;
    mReserved=0;
}

template <typename T>
OptimalList<T>::~OptimalList()
{
    clear();
}

template <typename T>
void OptimalList<T>::clear()
{
    if (isLarge)
    {
        for (int i=0; i<mCount; ++i)
        {
            delete *reinterpret_cast<T**>(pointerAt(i));
        }
    }

    mBegin=0;
    mCount=0;
    mReserved=0;
    setOptimalCapacity();
}

template <typename T>
void OptimalList<T>::reserve(int size)
{
    if (mReserved!=size)
    {
        mReserved=size;
        setOptimalCapacity();
    }
}

template <typename T>
void OptimalList<T>::prepend(const T &t)
{
    ++mCount;

    if (mBegin==0)
    {
        setOptimalCapacity();

        memmove(pointerAt(1), pointerAt(0), (mCount-1)*sizeOfElement);
    }
    else
    {
        mBegin--;
    }

    if (isLarge)
    {
        *reinterpret_cast<T**>(pointerAt(0))=new T(t);
    }
    else
    {
        *reinterpret_cast<T*>(pointerAt(0))=t;
    }
}

template <typename T>
void OptimalList<T>::append(const T &t)
{
    ++mCount;
    setOptimalCapacity();

    if (isLarge)
    {
        *reinterpret_cast<T**>(pointerAt(mCount-1))=new T(t);
    }
    else
    {
        *reinterpret_cast<T*>(pointerAt(mCount-1))=t;
    }
}

template <typename T>
void OptimalList<T>::insert(int i, const T &t)
{
    if (i<=0)
    {
        prepend(t);
        return;
    }

    if (i>=mCount)
    {
        append(t);
        return;
    }

    ++mCount;

    if (mBegin==0 || i>(mCount >> 1))
    {
        setOptimalCapacity();
        memmove(pointerAt(i+1), pointerAt(i), (mCount-i-1)*sizeOfElement);
    }
    else
    {
        --mBegin;
        memmove(pointerAt(0), pointerAt(1), i*sizeOfElement);
    }

    if (isLarge)
    {
        *reinterpret_cast<T**>(pointerAt(i))=new T(t);
    }
    else
    {
        *reinterpret_cast<T*>(pointerAt(i))=t;
    }
}


template <typename T>
void OptimalList<T>::replace(int i, const T &t)
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::replace", "index out of range");

    if (isLarge)
    {
        **reinterpret_cast<T**>(pointerAt(i))=t;
    }
    else
    {
        *reinterpret_cast<T*>(pointerAt(i))=t;
    }
}

template <typename T>
void OptimalList<T>::removeAt(int i)
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::removeAt", "index out of range");

    if (isLarge)
    {
        delete *reinterpret_cast<T**>(pointerAt(i));
    }

    if (i>mCount >> 1)
    {
        memmove(pointerAt(i), pointerAt(i+1), (mCount-i-1)*sizeOfElement);
    }
    else
    {
        memmove(pointerAt(1), pointerAt(0), i*sizeOfElement);

        mBegin++;
    }

    --mCount;
    setOptimalCapacity();
}

template <typename T>
void OptimalList<T>::removeFirst()
{
    if (mCount>0)
    {
        removeAt(0);
    }
}

template <typename T>
void OptimalList<T>::removeLast()
{
    if (mCount>0)
    {
        removeAt(mCount-1);
    }
}

template <typename T>
int OptimalList<T>::removeAll(const T &t)
{

}

template <typename T>
bool OptimalList<T>::removeOne(const T &t)
{

}

template <typename T>
T OptimalList<T>::takeAt(int i)
{

}

template <typename T>
T OptimalList<T>::takeFirst()
{

}

template <typename T>
T OptimalList<T>::takeLast()
{

}

template <typename T>
const T &OptimalList<T>::at(int i) const
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::at", "index out of range");

    if (isLarge)
    {
        return **reinterpret_cast<T**>(pointerAt(i));
    }
    else
    {
        return *reinterpret_cast<T*>(pointerAt(i));
    }
}

template <typename T>
const T &OptimalList<T>::operator[](int i) const
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::operator[]", "index out of range");

    if (isLarge)
    {
        return **reinterpret_cast<T**>(pointerAt(i));
    }
    else
    {
        return *reinterpret_cast<T*>(pointerAt(i));
    }
}

template <typename T>
T &OptimalList<T>::operator[](int i)
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::operator[]", "index out of range");

    if (isLarge)
    {
        return **reinterpret_cast<T**>(pointerAt(i));
    }
    else
    {
        return *reinterpret_cast<T*>(pointerAt(i));
    }
}

template <typename T>
int OptimalList<T>::length() const
{
    return mCount;
}

template <typename T>
int OptimalList<T>::size() const
{
    return mCount;
}

template <typename T>
int OptimalList<T>::count() const
{
    return mCount;
}

template <typename T>
bool OptimalList<T>::isEmpty() const
{
    return mCount==0;
}

template <typename T>
void OptimalList<T>::move(int from, int to)
{

}

template <typename T>
void OptimalList<T>::swap(int i, int j)
{

}

template <typename T>
void OptimalList<T>::swap(OptimalList<T> &other)
{

}

template <typename T>
int OptimalList<T>::indexOf(const T &t, int from) const
{

}
template <typename T>
int OptimalList<T>::lastIndexOf(const T &t, int from) const
{

}

template <typename T>
QBool OptimalList<T>::contains(const T &t) const
{

}

template <typename T>
int OptimalList<T>::count(const T &t) const
{

}

template <typename T>
OptimalList<T> &OptimalList<T>::operator=(const OptimalList<T> &l)
{
    clear();

    mCount=l.mCount;
    mCapacity=l.mCount;
    mBuffer = malloc(mCapacity*sizeOfElement);

    if (isLarge)
    {
        for (int i=0; i<mCount; ++i)
        {
            *reinterpret_cast<T**>(pointerAt(i))=new T(**reinterpret_cast<T**>(l.pointerAt(i)));
        }
    }
    else
    {
        for (int i=0; i<mCount; ++i)
        {
            *reinterpret_cast<T*>(pointerAt(i))=*reinterpret_cast<T*>(l.pointerAt(i));
        }
    }

    return *this;
}

template <typename T>
bool OptimalList<T>::operator==(const OptimalList<T> &l) const
{
    if (mCount != l.mCount)
    {
        return false;
    }

    if (mBuffer == l.mBuffer)
    {
        return true;
    }

    if (isLarge)
    {
        for (int i=0; i<mCount; ++i)
        {
            if (**reinterpret_cast<T**>(pointerAt(i)) != **reinterpret_cast<T**>(l.pointerAt(i)))
            {
                return false;
            }
        }
    }
    else
    {
        for (int i=0; i<mCount; ++i)
        {
            if (*reinterpret_cast<T*>(pointerAt(i)) != *reinterpret_cast<T*>(l.pointerAt(i)))
            {
                return false;
            }
        }
    }

    return true;
}

template <typename T>
bool OptimalList<T>::operator!=(const OptimalList<T> &l) const
{
    return !(*this == l);
}

template <typename T>
void OptimalList<T>::setOptimalCapacity()
{
    int aNewCapacity=mCapacity;
    int aSize=mCount+mBegin;

    if (aSize>aNewCapacity)
    {
        if (aNewCapacity==0)
        {
            aNewCapacity=aSize;
        }
        else
        {
            if (aNewCapacity<mReserved)
            {
                aNewCapacity=mReserved;
            }

            do
            {
                if (aSize<=aNewCapacity)
                {
                    break;
                }

                aNewCapacity = aNewCapacity << 1;
            } while (true);
        }
    }
    else
    if (mCount<aNewCapacity)
    {
        do
        {
            int aTempCapacity=aNewCapacity >> 1;

            if (mCount>aTempCapacity || mReserved>aTempCapacity)
            {
                break;
            }

            aNewCapacity=aTempCapacity;

            if (aNewCapacity==0)
            {
                break;
            }
        } while (true);

        if (mCapacity!=aNewCapacity && mBegin!=0)
        {
            int aBegin=mBegin;
            mBegin=0;

            memmove(pointerAt(0), pointerAt(aBegin), mCount*sizeOfElement);
        }
    }

    if (mCapacity!=aNewCapacity)
    {
        mCapacity=aNewCapacity;

        if (mCapacity==0)
        {
            free(mBuffer);
            mBuffer=0;
        }
        else
        {
            mBuffer = realloc(mBuffer, mCapacity*sizeOfElement);
            Q_CHECK_PTR(mBuffer);
        }
    }
}

template <typename T>
void* OptimalList<T>::pointerAt(const int i)
{
    return (void*)((long)mBuffer+(mBegin+i)*sizeOfElement);
}

#endif // OPTIMALLIST_H
