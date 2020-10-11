/* Proxy object.
 * @file
 * @date 2018-08-07
 * @author Anonymous
 */

#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include <mutex>

template<class T>
class ptr_holder
{
public:
    ptr_holder(T* ptr): ptr_(ptr) {}

    //{ describe proxy object
    class proxy: private std::lock_guard<std::mutex>
    {
    public:
        proxy(T* data, std::mutex& mutex): std::lock_guard<std::mutex>
        (mutex), data_(data)
        {}
        T* operator -> () const {
        return data_;
        }

    private:
        T* data_;
    };

    proxy operator -> () const
    {
        return {ptr_, mutex_};
    }
    //}

private:
    T* ptr_;
    mutable std::mutex mutex_;
};

#endif // __PROXY_HPP__
