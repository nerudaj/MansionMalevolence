#pragma once

#include <vector>

template<class T>
class [[nodiscard]] EventQueue final
{
public:
    template<class EventType, class... Args>
    void pushEvent(Args&&... args)
    {
        events.template emplace_back<EventType>(
            EventType { std::forward<Args>(args)... });
    }

    template<class Visitor>
    void processEvents(Visitor&& visitor)
    {
        for (size_t idx = 0; idx < events.size(); ++idx)
        {
            std::visit(visitor, events[idx]);
        }

        events.clear();
    }

private:
    std::vector<T> events;
};
