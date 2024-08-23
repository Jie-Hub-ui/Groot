/***************************************************************************************************
exception.h:	
    C++ Foundation Library header files

Purpose:
    ESingleton design pattern

Author:
    Lei jie

Creating Time:
    2024-8-14
    
***************************************************************************************************/
#ifndef __NC_SINGLETON_H__
#define __NC_SINGLETON_H__
#include <iostream>
#include <memory>
#include <mutex>

static std::mutex _singletion_lock;

template <typename T>
class ncSingletonFactory
{
public:
    static T *instance()
    {
        T *single = new T();
        return single;
    }
};

template <typename T, typename MANA = ncSingletonFactory<T> >
class ncSingleton
{
private:
    ncSingleton(const ncSingleton &);
    const ncSingleton &operator= (const ncSingleton &);

protected:
    ncSingleton() { }
    ~ncSingleton() { }
    static T *_singleton;

public:
    static __inline void DelInstance(void)
    {
        std::unique_lock<std::mutex> lock(_singletion_lock);
        if (NULL != _singleton) {
            delete _singleton;
            _singleton = NULL;
        }
    }

    static __inline T *Instance(void)
    {
        std::unique_lock<std::mutex> lock(_singletion_lock);
        if (!_singleton) {
            _singleton = MANA::instance();
        }
        return _singleton;
    }

    static __inline T &GetInstance(void)
    {
        return *Instance();
    }
};

template <typename T, typename MANA>
T* ncSingleton<T, MANA>::_singleton = 0;


#endif // __NC_SINGLETON_H__