#ifndef ADRIENCOIN_UTIL_H
#define ADRIENCOIN_UTIL_H

#include <array>

namespace adr
{
    // Shifts all elements of an array to the right, removing the last element
    // Will zero-initialize the first element
    template<typename T, std::size_t N>
    void arrayShiftRight(std::array<T, N>& arr)
    {
        // This will iterate over every element except the last and the first
        for (std::size_t i{ N - 1 }; i > 0; --i) {
            arr[i] = arr[i - 1];
        }

        // Zero Initialize the first element
        arr[0] = T{};
    }
}

#endif
