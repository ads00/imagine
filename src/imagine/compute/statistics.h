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

#ifndef COMPUTE_STATISTICS_H
#define COMPUTE_STATISTICS_H

#include "imagine/compute/math.h"

namespace ig  {

template <typename T>
class moments
{
public:
  static_assert(std::is_arithmetic<T>::value,
                "Moments calculation requires arithmetic values");

  constexpr moments() 
    : size_{0}, m1_{0}, m2_{0}, m3_{0}, m4_{0} {}

  template <typename InputIt>
  moments(InputIt first, InputIt last) : moments{}
  {
    process(first, last);
  }

  template <typename InputIt>
  void process(InputIt first, InputIt last)
  {
    for (std::size_t i = size_; first != last; ++first, ++i)
    {
      const T d = (*first) - m1_;
      const T dn = d / ++size_;
      const T ds = dn * dn;
      const T t = d * dn * i;

      m1_ += dn;
      m4_ += t * ds * (size_*size_ - 3*size_ + 3) + 6*ds*m2_ - 4*dn*m3_;
      m3_ += t * dn * (size_ - 2) - 3*dn*m2_;
      m2_ += t;
    }
  }

  constexpr T mean() const { return m1_; }
  constexpr T variance() const { return m2_ / (size_ - 1); }
  constexpr T sd() const       { return std::sqrt(variance()); }
  constexpr T skewness() const { return std::sqrt(size_) * m3_ / std::pow(m2_, 1.5); }
  constexpr T kurtosis() const { return size_ * m4_ / (m2_ * m2_) - 3; }

private:
  std::size_t size_;
  T m1_, m2_, m3_, m4_;
};

} // namespace ig

#endif // COMPUTE_STATISTICS_H
