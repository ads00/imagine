/*
 Copyright (c) 2017
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

#ifndef IG_MATH_MOMENTS_H
#define IG_MATH_MOMENTS_H

#include "imagine/math/discrete.h"

namespace ig {

template <typename T = double>
class moments {
public:
  static_assert(std::is_arithmetic<T>::value, "Moments calculation requires arithmetic values");

  moments() = default;

  template <typename InputIt>
  explicit moments(InputIt first, InputIt last) : moments{} 
  { process(first, last); }

  template <typename InputIt>
  void process(InputIt first, InputIt last) {
    for (size_t i = c_; first != last; ++first, ++i) {
      auto d = *first - m1_;
      auto dn = d / ++c_;
      auto ds = dn * dn;
      auto t = d * dn * i;

      m1_ += dn;
      m4_ += t * ds * (c_ * c_ - 3 * c_ + 3) + 6 * ds * m2_ - 4 * dn * m3_;
      m3_ += t * dn * (c_ - 2) - 3 * dn * m2_;
      m2_ += t;
    }
  }

  constexpr auto mean() const     { return m1_; }
  constexpr auto variance() const { return m2_ / (c_ - T(1)); }
  constexpr auto skewness() const { return std::sqrt(c_) * m3_ / std::pow(m2_, T(1.5)); }
  constexpr auto kurtosis() const { return c_ * m4_ / (m2_ * m2_) - T(3); }

private:
  size_t c_;
  T m1_, m2_, m3_, m4_;
};

} // namespace ig

#endif // IG_MATH_MOMENTS_H
