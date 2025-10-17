#pragma once

template<class... Ts>
struct overloads : Ts...
{
    using Ts::operator()...;
};

#ifdef ANDROID

// Deduction guide not needed since C++20
template<class... Ts>
overloads(Ts...) -> overloads<Ts...>;

#endif
