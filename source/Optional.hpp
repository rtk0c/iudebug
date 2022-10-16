#pragma once

// A very basic optional, for default-constructible and copyable types.
// (usually PODs)
template <typename T>
struct Optional {
    T value;
    bool hasValue;
    
    Optional() : hasValue{ false } {}
    Optional(T t) : value{ t }, hasValue{ true } {}
    
    T OrElse(T that) {
        if (hasValue) {
            return value;
        } else {
            return that;
        }
    }
};
