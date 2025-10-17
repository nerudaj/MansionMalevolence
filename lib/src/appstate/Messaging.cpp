#include <appstate/Messaging.hpp>

template<class Head, class... Tail>
std::optional<AppMessage> deserializeFrom(const std::string& str)
{
    if (str == Messaging::serialize<Head>()) return Head {};
    if constexpr (sizeof...(Tail) > 0)
        return deserializeFrom<Tail...>(str);
    else
        return std::nullopt;
}

template<class>
struct DeserializationWrapper;

template<class... Ts>
struct DeserializationWrapper<std::variant<Ts...>>
{
    std::optional<AppMessage> deserialized;

    DeserializationWrapper(const std::string& str)
    {
        deserialized = deserializeFrom<Ts...>(str);
    }
};

std::optional<AppMessage> Messaging::deserialize(const std::string& str)
{
    return DeserializationWrapper<AppMessage>(str).deserialized;
}
