#pragma once

// #include <iostream>
// #include <stdio.h>

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


class Barrier
{
public:
  Barrier(const size_t count);
  virtual ~Barrier() = default;

  void Wait();
  // void Break();

private:
  std::condition_variable _cv;
  std::mutex _mtx;
  size_t _counter;
  size_t _waiting;
  const size_t _threadCount;
};


template<typename T>
class Matrix
{
public:
  using vector_t = typename std::vector<T>;
  using vector_s_t = typename std::vector<size_t>;
  using vector_ld_t = typename std::vector<ld_t>;
  using table_t = typename std::vector<vector_t>;
  using table_ld_t = typename std::vector<vector_ld_t>;

  enum Methods { LAPLACE, LU };

  virtual ~Matrix() = default;

  Matrix();
  Matrix(const size_t n);
  Matrix(const size_t n, const T val);
  Matrix(const std::initializer_list< std::initializer_list<T> >);

  vector_t& operator[](size_t i) { return data[i]; };
  // bool operator==(const Matrix&);

  const size_t size() const { return _size; };

  /*
    NOT CONST CAUSE OF SHARED PRIVATE VARIABLES
  */
  ld_t Determinant(size_t = 1, const Methods = LU);
  ld_t DeterminantLU(size_t = 1);
  ld_t DeterminantLaplace(size_t = 1);

private:
  table_t data;
  const size_t _size;

  size_t _perThread = 1;
  size_t _modThread = 0;
  size_t _threadsCount = 5;

  ld_t DetRecursive(size_t, size_t, size_t, vector_s_t&);
  void DetLU(table_ld_t&, vector_s_t&, ld_t&, Barrier&, Barrier&, const size_t = 0);
};


Barrier::Barrier(const size_t count) : _threadCount(count), _counter(0), _waiting(0) {};

void Barrier::Wait()
{
  std::unique_lock<std::mutex> lock(_mtx);
  ++_counter;
  ++_waiting;
  _cv.wait(lock, [&]() { return _counter >= _threadCount; });
  --_waiting;
  _cv.notify_one();
  if (_waiting == 0) _counter = 0;
  lock.unlock();
}

template<typename T>
Matrix<T>::Matrix() : data(0), _size(0) {};

template<typename T>
Matrix<T>::Matrix(const size_t n) : data(n, vector_t(n, T())), _size(n) {};

template<typename T>
Matrix<T>::Matrix(const size_t n, const T val) : data(n, vector_t(n, val)), _size(n) {};

template<typename T>
Matrix<T>::Matrix(const std::initializer_list< std::initializer_list<T> > d) : Matrix(d.size())
{
  size_t i = 0, j = 0;
  for (const auto& l : d) {
    for (const auto& v : l) {
      data[i][j] = v;
      ++j;
    }
    j = 0;
    ++i;
  }
};

template<typename T>
ld_t Matrix<T>::Determinant(size_t threadsCount, const Methods method)
{
  if (size() == 0) return 0;
  if (threadsCount < 1) threadsCount = 1;

  if (method == LAPLACE) {
    return DeterminantLaplace(threadsCount);
  }
  return DeterminantLU(threadsCount);
}

template<typename T>
ld_t Matrix<T>::DeterminantLU(size_t threadsCount)
{
  if (size() == 0) return 0;
  if (threadsCount < 1) threadsCount = 1;
  _threadsCount = threadsCount;

  Barrier s1(threadsCount), s2(threadsCount);
  ld_t det = 1;
  vector_s_t swap(_size);
  table_ld_t matrix(_size, vector_ld_t(_size, 0));
  for (auto i = 0; i < _size; ++i) {
    swap[i] = i;
    for (auto j = 0; j < _size; ++j) {
      matrix[i][j] = static_cast<ld_t>(data[i][j]);
    }
  }

  std::vector< std::thread > ths;
  for(size_t i = 0; i < threadsCount; ++i) {
    ths.emplace_back(std::move( std::thread(
      &Matrix<T>::DetLU, this, std::ref(matrix), std::ref(swap), std::ref(det), std::ref(s1), std::ref(s2), i
    ) ));
  }

  for (auto& thr : ths) {
    thr.join();
  }

  if (std::isnan(det)) {
    det = .0;
  }
  return det;
}

template<typename T>
ld_t Matrix<T>::DeterminantLaplace(size_t threadsCount)
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
      std::launch::async, &Matrix<T>::DetRecursive, this, asyncSize, _perThread + (_modThread > 0), 0, std::ref(used[i])
    ) ));
    asyncSize += _perThread + (_modThread-- > 0);
  }

  ld_t result = DetRecursive(asyncSize, _size - asyncSize, 0, used[0]);
  // printf("%Lf ... ", result);
  for (auto t = ths.begin(); t != ths.end(); ++t) {
    auto value = t->get();
    // printf("%Lf, ", value);
    result += value;
  }
  return result;
}

template<typename T>
void Matrix<T>::DetLU(
    table_ld_t&   matrix,
    vector_s_t&   swap,
    ld_t&         det,
    Barrier&      s1,
    Barrier&      s2,
    const size_t  threadNumber)
{
  for (auto k = 0; k < this->_size; ++k) {
    auto offset = k + 1;

    if (threadNumber == 0) {
      // find max pivot in k-column
      auto p = k;
      for (auto i = p + 1; i < this->_size; ++i) {
        if ( fabs(matrix[ swap[i] ][k]) - fabs(matrix[ swap[p] ][k]) > EPS ) {
          p = i;
        }
      }

      // swap rows
      auto foo = swap[k];
      swap[k] = swap[p];
      swap[p] = foo;

      // calc det
      auto pivot = matrix[ swap[k] ][k];
      for (auto i = offset; i < this->_size; ++i) {
        matrix[ swap[i] ][k] /= pivot;
      }
      det *= pivot * (2 * (k == p) - 1);

      auto tmpSize = this->_size - offset;
      this->_perThread = std::max(static_cast<size_t>(1), tmpSize / this->_threadsCount);
      this->_modThread = tmpSize - std::min(this->_perThread * this->_threadsCount, tmpSize);
    }

    s1.Wait();

    // update matrix
    size_t start = offset + this->_perThread * threadNumber + std::min(this->_modThread, threadNumber);
    size_t end = std::min(start + this->_perThread + (threadNumber < this->_modThread), this->_size);

    for (auto i = start; i < end; ++i) {
      double x = matrix[ swap[i] ][k];
      for (auto j = offset; j < this->_size; ++j) {
        matrix[ swap[i] ][j] -= x * matrix[ swap[k] ][j];
      }
    }

    s2.Wait();
    // if (threadNumber == 0) std::this_thread::sleep_for(std::chrono::milliseconds(5));
    // std::this_thread::sleep_for(std::chrono::nanoseconds(100));
  }
}

template<typename T>
ld_t Matrix<T>::DetRecursive(
    size_t       start,
    size_t       count,
    size_t       row,
    vector_s_t&  used)
{
  auto len = used.size();
  auto end = start + count;

  // if (row == 0) {
  //   printf("D[] %d -> %d\n", start, end);
  // }

  if (row >= this->_size) {
    // throw Exception();
    // printf("EXCEPTION SIZE\n");
    return 0;
  }

  if (end > len) {
    // throw Exception();
    // printf("EXCEPTION END\n");
    return 0;
  }

  if (row == this->_size - 1) {
    for (auto i = start; i < end; ++i) {
      if (used[i] == 0) return this->data[row][i];
    }
    // throw Exception();
    // printf("EXCEPTION AVAILABILITY: %d\n", used);
    return 0;
  }

  ld_t det = 0;
  int k = (1 - ((start & 1) << 1));
  for (auto i = start; i < end; ++i) {
    if (used[i] > 0) continue;

    used[i] = 1;
    det += k * this->data[row][i] * DetRecursive(0, len, row + 1, used);
    used[i] = 0;
    k = -k;
  }
  return det;
}

// template<typename T>
// T Matrix<T>::Determinant(vector_s_t& jumps, size_t dim)
// {
//   if (dim < 2) {
//     // printf("->[%d; %d](%d)", size - 1, jumps.at(jumps[0]), data[size - 1][jumps.at(jumps[0])]);
//     printf("%d [%d; %d; %d]\n", data[size - 1][jumps.at(jumps[0])], size - 1, jumps.at(jumps[0]), 0);
//     return data[size - 1][jumps.at(jumps[0])];
//   }

//   T d(0);
//   auto row = size - dim;
//   // auto foo = 0, bar = 0;
//   for (auto x = 0, i = 0; x < size; ++x, ++i) {
//     while (x != jumps[x]) {
//       x = jumps[x];
//     }
//     if (x >= size) break;
//     // while (foo != x) {
//     //   bar = foo;
//     //   foo = jumps[bar];
//     //   jumps[bar] = x;
//     // }

//     jumps[x] = jumps[x + 1];
//     // printf("->[%d; %d](%d)", row, x, data[row][x]);
//     printf("%d [%d; %d; %d]\n", (1 - ((i & 1) << 1)) * data[row][x], row, x, i);
//     auto k = Determinant(jumps, dim - 1);
//     d += (1 - ((i & 1) << 1)) * data[row][x] * k;
//     jumps[x] = x;
//     if (row < 2) {
//       printf("\n");
//       // printf("%d * %d\n", (1 - ((i & 1) << 1)) * data[row][x], k);
//     }
//   }
//   return d;
// }

// template<typename T>
// T Matrix<T>::Determinant(std::deque<size_t>& availableColumns)
// {
//   auto dim = availableColumns.size();
//   if (dim < 2) {
//     std::cout << "OUT: " << data[size - 1][availableColumns.back()] << " " << 1 << " " << availableColumns.back() << std::endl;
//     return data[size - 1][availableColumns.back()];
//   }

//   T d = 0;
//   auto row = size - dim;
//   for (auto i = 0; i < dim; ++i) {
//     auto x = availableColumns.front(); 
//     availableColumns.pop_front();
//     std::cout << "IN: " << data[size - dim][x] << " [" << ((x + row & 1) << 1) << "] " << x << std::endl;
//     d += (1 - ((x + row & 1) << 1)) * data[row][x] * Determinant(availableColumns);
//     availableColumns.push_back(x);
//   }
//   return d;
// }

} // namespace thrd
