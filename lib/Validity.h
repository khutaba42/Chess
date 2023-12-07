#pragma once

// comment out if you don't want the variadic constructor
// #define __CHESS_GAME__VALIDITY_STRUCT__VARIADIC_CONSTRUCTOR_ON__

#ifdef __CHESS_GAME__VALIDITY_STRUCT__VARIADIC_CONSTRUCTOR_ON__
#include <utility> // for std::forward<Args>
#endif

template <typename Obj>
struct Validity
{
    bool valid;
    Obj data;

#ifdef __CHESS_GAME__VALIDITY_STRUCT__VARIADIC_CONSTRUCTOR_ON__
    template <typename... Args>
    Validity(Args &&...args) : valid(true), data{std::forward<Args>(args)...} {}
#endif

    Validity() : valid(false), data{} {}
    Validity(const Obj &obj) : valid(true), data(obj) {}
};
