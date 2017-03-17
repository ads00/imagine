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

#ifndef IG_MATH_INTEGRATE_H
#define IG_MATH_INTEGRATE_H

#include "imagine/math/discrete.h"

namespace ig        {
namespace integrate {

template <typename T, typename Fn>
auto adaptive_simpson(Fn&& fn, T a, T b, T tolerance = 1e-10) {
  auto adapt = [&fn](auto& l, auto a, auto b, auto fa, auto fm, auto fb, auto is) -> T {
    auto m = (a + b) / 2.0;
    auto h = (b - a) / 4.0;
    auto fml = fn(a + h);
    auto fmr = fn(b - h);
    auto i = h / 3.0 * (fa + 4.0 * (fml + fmr) + 2.0 * fm + fb);
    auto j = 
      (16.0 * i - 
        h / 1.5 * (fa + 4.0 * fm + fb)
      ) / 15.0;

    return std::abs(j - i) <= std::abs(is) || (m <= a) || (b <= m)
      ? j
      : l(l, a, m, fa, fml, fm, is) + l(l, m, b, fm, fmr, fb, is);
  };

  auto ba = b - a;
  auto fa = fn(a);
  auto fb = fn(b);
  auto fm = fn((a + b) / 2.0);

  auto is = ba / 8.0 * (fa + fb + fm + fn(a + 0.9501 * ba) + fn(a + 0.2311 * ba) + fn(a + 0.6068 * ba) + fn(a + 0.4860 * ba) + fn(a + 0.8913 * ba));
  auto ri = is == 0
    ? b - a
    : is * tolerance;

  return adapt(adapt, a, b, fa, fm, fb, ri);
}

} // namespace integrate
} // namespace ig

#endif // IG_MATH_INTEGRATE_H
