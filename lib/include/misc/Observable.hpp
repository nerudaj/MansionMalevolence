#pragma once

#include <functional>

template<class T>
class Observable
{
public:
    Observable(T initial, std::function<void(T)> notifyCallback)
        : value(initial), notifyObserver(notifyCallback)
    {
        notifyObserver(value);
    }

public:
    Observable& operator=(T newValue)
    {
        value = newValue;
        notifyObserver(value);
        return *this;
    }

    operator T() const // NOLINT
    {
        return value;
    }

private:
    T value;
    std::function<void(T)> notifyObserver;
};
