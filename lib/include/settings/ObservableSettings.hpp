#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include <vector>

template<class T>
class [[nodiscard]] ObservableSettings final
{
public:
    friend void
    to_json(nlohmann::json& j, const ObservableSettings<T>& settings)
    {
        j = settings.data;
    }

    friend void
    from_json(const nlohmann::json& j, ObservableSettings<T>& settings)
    {
        settings.data = j;
    }

public:
    const T& get() const noexcept
    {
        return data;
    }

    void set(T newData)
    {
        data = newData;
        for (auto&& observer : observers)
            observer(data);
    }

    void registerObserver(std::function<void(const T&)> observer)
    {
        observers.push_back(observer);
    }

private:
    T data;
    std::vector<std::function<void(const T&)>> observers;
};
