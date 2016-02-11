/*
 Copyright (c) 2015, 2016
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#ifndef COMPUTE_MOMENTS_H
#define COMPUTE_MOMENTS_H

#include "imagine/compute/math.h"

namespace ig         {
namespace statistics {

template <typename InputIt>
auto mean(InputIt first, InputIt last)
{
  const auto size = last - first;

  typename std::iterator_traits<InputIt>::value_type m = 0;
  for (; first != last; ++first) m += *(first);

  return m / size;
}

template <typename InputIt>
auto variance(InputIt first, InputIt last, bool biased = true)
{
  const auto size = last - first;
  const auto m = mean(first, last);

  typename std::iterator_traits<InputIt>::value_type v = 0;
  for (; first != last; ++first)
  {
    const auto x = *(first) - m;
    v += x * x;
  }

  return v / (biased ? (size - 1) : size); 
}

template <typename InputIt>
auto standard_derivation(InputIt first, InputIt last, bool biased = true)
{
  return std::sqrt(variance(first, last, biased));
}

template <typename InputIt>
auto skewness(InputIt first, InputIt last, bool biased = true)
{
  const auto size = last - first;
  const auto m = mean(first, last);

  typename std::iterator_traits<InputIt>::value_type s2 = 0, s3 = 0;
  for (; first != last; ++first)
  {
    const auto d = *(first) - m;
    s2 += d * d; s3 += d * d * d;
  }

  s2 /= size; s3 /= size;
  const auto g = s3 / (std::pow(s2, 1.5));

  return biased ? (std::sqrt(size * (size - 1)) / (size - 2)) * g : g;
}

template <typename InputIt>
auto kurtosis(InputIt first, InputIt last, bool biased = true)
{
  const auto size = last - first;
  const auto m = mean(first, last);
  
  typename std::iterator_traits<InputIt>::value_type k2 = 0, k4 = 0;
  for (; first != last; ++first)
  {
    const auto d = *(first) - m;
    k2 += d * d; k4 += d * d * d * d;
  }

  if (biased)
  {
    const auto v = k2 / (size - 1);

    const decltype(k2) a = (size * (size + 1));
    const decltype(k2) b = (size - 1) * (size - 1);
    return a / ((size - 1) * (size - 2) * (size - 3)) * k4 / (v * v) - 3*(b / ((size - 2) * (size - 3)));
  }
  else
  {
    const auto m2 = k2 / size;
    return (k4 / size) / (m2 * m2) - 3;
  }
}

} // namespace stats
} // namespace ig

#endif // COMPUTE_MOMENTS_H
