#ifndef RANDOMSTORAGE_H
#define RANDOMSTORAGE_H

#include <cmath>

#include <algorithm>
#include <mutex>

namespace average {

template<typename T>
class RandomStorage
{
public:
    static double average(const T& t)
    {
        int acc = 0;
        std::for_each(std::begin(t), std::end(t), [&acc](auto&& value) {
            acc += std::pow(value, 2);
        });
        return static_cast<double>(acc) / t.size();
    }
public:
    void insert(int value)
    {
        std::lock_guard lock(_mutex);
        _data.insert(value);
    }
    double average() const
    {
        std::lock_guard lock(_mutex);
        return average(_data);
    }
    T copy_data() const
    {
        std::scoped_lock lock(_mutex);
        return _data;
    }
protected:
    T _data;
    mutable std::mutex _mutex;
};

} // namespace average

#endif // RANDOMSTORAGE_H
