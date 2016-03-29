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

#ifndef COMPUTE_SVD_H
#define COMPUTE_SVD_H

#include "imagine/compute/linalg/matrix.h"
#include "imagine/compute/linalg/solver.h"

namespace ig
{

/* 
 Compute a SVD factorization of a general M-by-N matrix A

 The factorization has the form
   A = U D V^T

 - U is a column-orthogonal M-by-N matrix
 - D is a diagonal N-by-N matrix
 - V is an orthogonal N-by-N matrix

 U is stored in a computed matrix and contains the left singular vectors

 D is stored in a vector, and contains the singular values

 V is stored in a computed matrix and contains the right singular vectors
*/

template <typename Alg>
class svd
{
public:
  using T = alg_t<Alg>;
  using matrix_t = matrix<T>;
  using vector_t = vector<T>;

  static_assert(std::is_arithmetic<T>::value, 
                "Singular value decomposition requires an arithmetic matrix");

  svd(const matrix_t& alg);

  std::size_t rank() const;
  matrix_t pinv() const;
  vector_t solve(const vector_t& b);

  constexpr const matrix_t& matrixU() const { return U_; }
  constexpr const matrix_t& matrixV() const { return V_; }
  constexpr const vector_t& singular_values() const { return S_; }

private:
  const std::size_t M_;
  const std::size_t N_;

  T threshold_;

  matrix_t U_;
  matrix_t V_;
  vector_t S_;
};

template <typename Alg>
svd<Alg>::svd(const matrix_t& alg)
  : M_{alg.rows()}, N_{alg.cols()}, threshold_{T(0)}, U_ {alg}, V_{N_, N_}, S_{N_}
{
  vector_t work{N_};
  T g = T(0);

  // Householder's reduction to bidiagonal form
  for (std::size_t i = 0; i < N_; ++i)
  {
    const std::size_t l = i + 1;
    work[i] = g;

    T s1 = T(0);
    for (std::size_t j = i; j < M_; ++j) s1 += U_(j, i) * U_(j, i);

    if (s1 > eps<T>)
    {
      T f = U_(i, i);
      g = -sign(f) * std::sqrt(s1);

      const T h = f*g - s1;
      U_(i, i) = f - g;

      for (std::size_t j = l; j < N_; ++j)
      {
        T s = T(0);
        for (std::size_t k = i; k < M_; ++k) s += U_(k, i) * U_(k, j);

        f = s / h;
        for (std::size_t k = i; k < M_; ++k) U_(k, j) += f * U_(k, i);
      }
    }
    else g = T(0);

    S_[i] = g;

    if (i >= M_) continue;

    T s2 = T(0);
    for (std::size_t j = l; j < N_; ++j) s2 += U_(i, j) * U_(i, j);

    if (s2 > eps<T>)
    {
      const T f = U_(i, l);
      g = -sign(f) * std::sqrt(s2);
      const T h = f*g - s2;

      U_(i, l) = f - g;
      for (std::size_t j = l; j < N_; ++j) work[j] = U_(i, j) / h;

      for (std::size_t j = l; j < M_; ++j)
      {
        T s = T(0);
        for (std::size_t k = l; k < N_; ++k) s += U_(j, k) * U_(i, k);
        for (std::size_t k = l; k < N_; k++) U_(j, k) += s * work[k];
      }
    }
    else g = T(0);
  }

  // Accumulation of right hand transformations
  for (std::size_t i = N_; i--> 0;)
  {
    const std::size_t l = i + 1;
    if (g != T(0))
    {
      const T h = g * U_(i, l);
      for (std::size_t j = l; j < N_; ++j) V_(j, i) = U_(i, j) / h;

      for (std::size_t j = l; j < N_; ++j)
      {
        T s = T(0);
        for (std::size_t k = l; k < N_; ++k) s += U_(i, k) * V_(k, j);
        for (std::size_t k = l; k < N_; ++k) V_(k, j) += s * V_(k, i);
      }
    }

    for (std::size_t j = l; j < N_; ++j) V_(i, j) = V_(j, i) = T(0);

    V_(i, i) = T(1);
    g = work[i];
  }

  // Accumulation of left hand transformations
  for (std::size_t i = std::min(M_, N_); i--> 0;)
  {
    const std::size_t l = i + 1;
    for (std::size_t j = l; j < N_; ++j) U_(i, j) = T(0);

    T g = S_[i];
    if (g != T(0))
    {
      const T h = U_(i, i) * g;
      for (std::size_t j = l; j < N_; ++j)
      {
        T s = T(0);
        for (std::size_t k = l; k < M_; ++k) s += U_(k, i) * U_(k, j);

        const T f = s / h;
        for (std::size_t k = i; k < M_; ++k) U_(k, j) += f * U_(k, i);
      }

      for (std::size_t j = i; j < M_; ++j) U_(j, i) /= g;
    }
    else for (std::size_t j = i; j < M_; ++j)
      U_(j, i) = T(0);

    ++U_(i, i);
  }

  std::size_t sweeps = 75;
  threshold_ = eps<T> * std::max(M_, N_) * std::abs(S_[0]);

  // Diagonalization of the bidiagonal form
  for (std::size_t i = N_; i--> 0; )
  {
    std::size_t l = 0, nm = 0;
    for (std::size_t it = 0; it <= sweeps; ++it)
    {
      // Test for splitting
      bool split = true;
      for (l = i + 1; l--> 0; )
      {
        nm = l - 1;
        if (l == 0 || std::abs(work[l]) <= threshold_)
        {
          split = false;
          break;
        }

        if (std::abs(S_[nm]) <= threshold_)
          break;
      }

      if (split) 
      {
        T c = T(0);
        T s = T(1);
        for (std::size_t j = l; j < i + 1; ++j)
        {
          const T f = s * work[j];
          if (std::abs(f) <= threshold_) break;

          const T g = S_[j];
          const T h = std::hypot(f, g);

          S_[j] = h;
          c = g / h;
          s = -f / h;
          for (std::size_t k = 0; k < M_; ++k)
          {
            const T y = U_(k, nm);
            const T z = U_(k, j);

            U_(k, nm) = y*c + z*s;
            U_(k, j)  = z*c - y*s;
          }
        }
      }

      const T z = S_[i];

      // Convergence
      if (l == i) 
      {
        if (z < T(0))
        {
          S_[i] = -z;
          for (std::size_t j = 0; j < N_; ++j) V_(j, i) = -V_(j, i);
        }

        break;
      }

      // No convergence after x sweeps
      if (it == sweeps)
      {
        throw std::logic_error
          ("Singular value decomposition failed (No convergence)");
      }

      // Shift from bottom 2x2 minor
      const T y = S_[i - 1];
      const T h = work[i];
      T g = work[i - 1];

      T x = S_[l];
      T f = ((y - z)*(y + z) + (g - h)*(g + h)) / (T(2) * h * y);
      g = std::hypot(f, T(1));
      f = ((x - z)*(x + z) + h*((y / (f + sign(f)*g)) - h)) / x;

      T c = T(1);
      T s = T(1);

      // Next QR transformation
      for (std::size_t j = l; j < i; ++j)
      {
        const std::size_t k = j + 1;

        const T g1 = c * work[k];
        const T h1 = s * work[k];
        const T y1 = S_[k];

        const T z1 = std::hypot(f, h1);
        work[j] = z1;
        c = f / z1;
        s = h1 / z1;
        f = x*c + g1*s;

        const T g2 = g1*c - x*s;
        const T h2 = y1 * s;
        const T y2 = y1 * c;

        for (std::size_t jj = 0; jj < N_; ++jj)
        {
          const T x = V_(jj, j);
          const T z = V_(jj, k);

          V_(jj, j) = x*c + z*s;
          V_(jj, k) = z*c - x*s;
        }

        const T z2 = std::hypot(f, h2);
        S_[j] = z2;

        if (z2 != T(0)) 
        {
          c = f / z2;
          s = h2 / z2;
        }

        f = c*g2 + s*y2;
        x = c*y2 - s*g2;

        for (std::size_t jj = 0; jj < M_; ++jj)
        {
          const T y = U_(jj, j);
          const T z = U_(jj, k);

          U_(jj, j) = y*c + z*s;
          U_(jj, k) = z*c - y*s;
        }
      }

      work[l] = T(0);
      work[i] = f;
      S_[i] = x;
    }
  }
}

template <typename Alg>
std::size_t svd<Alg>::rank() const
{
  // Lookup for singular values > threshold 
  std::size_t r = T(0);
  for (std::size_t i = 0; i < N_; ++i)
    if (S_[i] > threshold_) ++r;

  return r;
}

template <typename Alg>
auto svd<Alg>::pinv() const -> matrix_t
{
  // Compute w = VS+
  matrix_t W{N_, N_};
  for (std::size_t i = 0; i < N_; ++i)
    for (std::size_t j = 0; j < N_; ++j)
      if (std::abs(S_[j]) > threshold_)
        W(i, j) = V_(i, j) * (T(1) / S_[j]);

  // Compute inv = WU^T
  return W * U_.t();
}

template <typename Alg>
auto svd<Alg>::solve(const vector_t& b) -> vector_t
{
  // Compute w = U^Tb
  vector_t w = U_.t() * b;

  // Apply singularity
  for (std::size_t i = 0; i < N_; ++i)
  {
    const T sv = S_[i];
    const T alpha = (std::abs(sv) > threshold_) ? T(1) / sv : sv;
    w[i] *= alpha;
  }

  // Compute x = Vw
  return V_ * w;
}

namespace linalg
{

template <typename Alg>
constexpr svd<Alg> svd_run(const alg<Alg>& alg)
{
  return svd<Alg>(alg);
}

} // namespace linalg
} // namespace ig

#endif // COMPUTE_SVD_H
