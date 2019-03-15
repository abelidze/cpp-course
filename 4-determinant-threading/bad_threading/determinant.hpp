#pragma once

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <cmath>
#include <condition_variable>

namespace thrd {

typedef long double ld_t;
const ld_t EPS = 1e-8;

template<typename T>
class Matrix
{
public:
  using vector_t = typename std::vector<T>;
  using vector_s_t = typename std::vector<size_t>;
  using table_t = typename std::vector<vector_t>;

  Matrix();
  Matrix(const size_t n);
  Matrix(const size_t n, const T val);
  Matrix(const std::initializer_list< std::initializer_list<T> >);
  virtual ~Matrix() = default;

  vector_t& operator[](size_t i) { return data[i]; }

  const size_t size() const { return _size; }

  ld_t Determinant(size_t = 1);

private:
  table_t data;
  const size_t _size;

  size_t _perThread = 1;
  size_t _modThread = 0;
  size_t _threadsCount = 5;

  ld_t DetJob(size_t, size_t, size_t, vector_s_t&);
};


Matrix<long double> RandomMatrix(size_t n)
{
  srand( time(0) );
  thrd::Matrix<long double> M(n);
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      M[i][j] = rand() % 10;
  return M;
}


template<typename T>
Matrix<T>::Matrix() : data(0), _size(0) {}

template<typename T>
Matrix<T>::Matrix(const size_t n) : data(n, vector_t(n, T())), _size(n) {}

template<typename T>
Matrix<T>::Matrix(const size_t n, const T val) : data(n, vector_t(n, val)), _size(n) {}

template<typename T>
Matrix<T>::Matrix(const std::initializer_list< std::initializer_list<T> > d) : Matrix(d.size())
{
  size_t i = 0, j = 0;
  for (auto& l : d) {
    for (auto& v : l) {
      data[i][j] = v;
      ++j;
    }
    j = 0;
    ++i;
  }
}

template<typename T>
ld_t Matrix<T>::Determinant(size_t threadsCount)
{
  if (size() == 0) return 0;
  if (threadsCount < 1) threadsCount = 1;
  _threadsCount = threadsCount;

  _perThread = std::max(static_cast<size_t>(1), size() / threadsCount);
  _modThread = _size - std::min(_perThread * threadsCount, _size);
  size_t asyncSize = 0;

  std::vector< vector_s_t > used(threadsCount, vector_s_t(_size, 0));

  std::vector< std::future<ld_t> > ths;
  for (auto i = 1; i < threadsCount && asyncSize < _size - 1; ++i) {
    ths.emplace_back(std::move( std::async(
      std::launch::async, &Matrix<T>::DetJob, this, asyncSize, _perThread + (_modThread > 0), 0, std::ref(used[i])
    ) ));
    asyncSize += _perThread + (_modThread-- > 0);
  }

  ld_t result = DetJob(asyncSize, _size - asyncSize, 0, used[0]);
  for (auto t = ths.begin(); t != ths.end(); ++t) {
    auto value = t->get();
    result += value;
  }
  return result;
}

template<typename T>
ld_t Matrix<T>::DetJob(
    size_t       start,
    size_t       count,
    size_t       row,
    vector_s_t&  used)
{
  auto len = used.size();
  auto end = start + count;

  if (row >= this->_size) {
    return 0;
  }

  if (end > len) {
    return 0;
  }

  if (row == this->_size - 1) {
    for (auto i = start; i < end; ++i) {
      if (used[i] == 0) return this->data[row][i];
    }
    return 0;
  }

  ld_t det = 0;
  int k = (1 - ((start & 1) << 1));
  for (auto i = start; i < end; ++i) {
    if (used[i] > 0) continue;

    used[i] = 1;
    det += k * this->data[row][i] * DetJob(0, len, row + 1, used);
    used[i] = 0;
    k = -k;
  }
  return det;
}

} // namespace thrd
