/** \file
 * \brief Minimum Window Substring implementation.
 */

#ifndef ALGORITHMS_MIN_WINDOW_SUBSTR_HPP_INCLUDED
#define ALGORITHMS_MIN_WINDOW_SUBSTR_HPP_INCLUDED

#include <utility>
#include <iterator>
#include <type_traits>
#include <unordered_map>

/// Algorithms namespace.
namespace algos {

/// Return type of minWindowSubstr() function.
template <typename Iter>
using ReturnType = std::tuple<Iter, Iter, std::size_t>;

/// Find minimum window substring containing all unique elements of the input range.
/**
 * Complexity:
 * - Time:  O(n)
 * - Space: O(k)
 *
 * where:
 * - n - size of the input range
 * - k - number of unique elements in the input range
 *
 * \tparam Iter iterator type, must meet the requirements of
 *   <a href="https://en.cppreference.com/w/cpp/named_req/ForwardIterator">LegacyForwardIterator</a>
 * \param first begin iterator of the range
 * \param last end (one-past-last) iterator of the range
 *
 * \return
 *   \parblock
 *     `tuple(window-start-iter, window-end-iter, window-length)` representing the minimum substring found
 *
 *      \c window-end-iter is an iterator to one-past-last element of the window
 *   \endparblock
 */
template <typename Iter>
ReturnType<Iter> minWindowSubstr(Iter first, Iter last) {
    static_assert(std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iter>::iterator_category>);
    // or is_convertible_v ?

    using value_type = typename std::iterator_traits<Iter>::value_type;
    std::unordered_map<value_type, std::size_t> elementCounts;
    // const iter vs const_iter !
    for (/*const*/auto it = first; it != last; it = std::next(it)) { //TODO std::advance instead of std::next
        elementCounts.try_emplace(*it, 0); //vs. insert({*it, 0})
    }
    std::size_t uniqueElems = elementCounts.size(), elemsPresent = 0;

    Iter wStart, wEnd;
    std::size_t wLength;
    bool initialized = false;
    wStart = wEnd = last;
    wLength = 0;

    Iter currStart = first;  // current start inclusive
    Iter newPos = currStart; // current end inclusive
    for(; newPos != last; newPos = std::next(newPos)) {
        if (wLength == uniqueElems) { // found the first minimal window
            break;
        }

        const auto count = ++elementCounts.at(*newPos);
        if (count == 1) {
            ++elemsPresent;
        }
        while (elemsPresent == uniqueElems) {
            std::size_t currLength = std::distance(currStart, newPos) + 1;
            if (currLength < wLength || !initialized) { // found shorter window
                initialized = true;
                wLength = currLength;
                wStart = currStart;
                wEnd = std::next(newPos);
            }
            //if (wLength == uniqueElems) // found the first minimal window

            const auto count = --elementCounts.at(*currStart);
            if (count == 0) {
                --elemsPresent;
            }
            currStart = std::next(currStart);
        }
    }

    return {wStart, wEnd, wLength};
}

//TODO lastMinWindowSubstr()
// easy way: reverse iterator

//TODO minWindowSubstrNoPreproc()
// without first iteration over all elements to put in the map

} // namespace algos

#endif // ALGORITHMS_MIN_WINDOW_SUBSTR_HPP_INCLUDED
