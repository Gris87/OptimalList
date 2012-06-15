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

    void append(const T &t);
    void insert(int i, const T &t);
    void removeAt(int i);
    void removeLast();

    const T &at(int i) const;
    const T &operator[](int i) const;
    T &operator[](int i);

    int length() const;
    int size() const;
    int count() const;

protected:
    enum
    {
        isLarge       = QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic,
        sizeOfElement = isLarge ? sizeof(void *) : sizeof(T)
    };

    void* mBuffer;
    int   mCount;
    int   mCapacity;

    void setOptimalCapacity();
};

template <typename T>
OptimalList<T>::OptimalList()
{
    mBuffer=0;
    mCount=0;
    mCapacity=0;

   // mIsLarge=QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic;
/*
    if (mIsLarge)
    {
        mSizeOfT=sizeof(void *);
    }
    else
    {
        mSizeOfT=sizeof(T);
    }*/
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
            delete *reinterpret_cast<T**>((long)mBuffer+i*sizeOfElement);
        }
    }

    mCount=0;
    setOptimalCapacity();
}

template <typename T>
void OptimalList<T>::append(const T &t)
{
    ++mCount;
    setOptimalCapacity();

    if (isLarge)
    {
        *reinterpret_cast<T**>((long)mBuffer+(mCount-1)*sizeOfElement)=new T(t);
    }
    else
    {
        *reinterpret_cast<T*>((long)mBuffer+(mCount-1)*sizeOfElement)=t;
    }
}

template <typename T>
void OptimalList<T>::insert(int i, const T &t)
{
    Q_ASSERT_X(i >= 0 && i <= mCount, "OptimalList<T>::insert", "index out of range");

    ++mCount;
    setOptimalCapacity();

    memmove((long)mBuffer+(i+1)*sizeOfElement, (long)mBuffer+i*sizeOfElement, (mCount-i-1)*sizeOfElement);

    if (isLarge)
    {
        *reinterpret_cast<T**>((long)mBuffer+i*sizeOfElement)=new T(t);
    }
    else
    {
        *reinterpret_cast<T*>((long)mBuffer+i*sizeOfElement)=t;
    }
}

template <typename T>
void OptimalList<T>::removeAt(int i)
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::removeAt", "index out of range");

    if (isLarge)
    {
        delete *reinterpret_cast<T**>((long)mBuffer+i*sizeOfElement);
    }

    memmove((long)mBuffer+i*sizeOfElement, (long)mBuffer+(i+1)*sizeOfElement, (mCount-i-1)*sizeOfElement);

    --mCount;
    setOptimalCapacity();
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
const T &OptimalList<T>::at(int i) const
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::at", "index out of range");

    if (isLarge)
    {
        return **reinterpret_cast<T**>((long)mBuffer+i*sizeOfElement);
    }
    else
    {
        return *reinterpret_cast<T*>((long)mBuffer+i*sizeOfElement);
    }
}

template <typename T>
const T &OptimalList<T>::operator[](int i) const
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::operator[]", "index out of range");

    if (isLarge)
    {
        return **reinterpret_cast<T**>((long)mBuffer+i*sizeOfElement);
    }
    else
    {
        return *reinterpret_cast<T*>((long)mBuffer+i*sizeOfElement);
    }
}

template <typename T>
T &OptimalList<T>::operator[](int i)
{
    Q_ASSERT_X(i >= 0 && i < mCount, "OptimalList<T>::operator[]", "index out of range");

    if (isLarge)
    {
        return **reinterpret_cast<T**>((long)mBuffer+i*sizeOfElement);
    }
    else
    {
        return *reinterpret_cast<T*>((long)mBuffer+i*sizeOfElement);
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
void OptimalList<T>::setOptimalCapacity()
{
    int aNewCapacity=mCapacity;

    if (mCount<aNewCapacity)
    {
        do
        {
            int aTempCapacity=aNewCapacity >> 1;

            if (mCount>aTempCapacity)
            {
                break;
            }

            aNewCapacity=aTempCapacity;

            if (aNewCapacity==0)
            {
                break;
            }
        } while (true);
    }
    else
    if (mCount>aNewCapacity)
    {
        if (aNewCapacity==0)
        {
            aNewCapacity=mCount;
        }
        else
        {
            do
            {
                if (mCount<=aNewCapacity)
                {
                    break;
                }

                aNewCapacity = aNewCapacity << 1;
            } while (true);
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

#endif // OPTIMALLIST_H
