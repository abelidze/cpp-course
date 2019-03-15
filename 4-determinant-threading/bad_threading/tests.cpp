#include <cmath>
#include <thread>

#include "catch.hpp"
#include "determinant.hpp"

const int THREADS_COUNT = std::thread::hardware_concurrency();
const long double EPS = 1e-14;

TEST_CASE("Predefined samples") {

  SECTION("Check sample 1") {
    thrd::Matrix<long double> M({
      { 1, 0, 2, 0, 1, 2 },
      { 0, 0, 3, 0, 0, 1 },
      { 0, 3, 0, 2, 2, 0 },
      { 4, 0, 7, 0, 6, 2 },
      { 0, 3, 0, 3, 0, 0 },
      { 0, 1, 2, 0, 0, 4 }
    });
    auto noThreads = M.Determinant();
    auto withThreads = M.Determinant(THREADS_COUNT);
    REQUIRE( std::abs(noThreads - (-42)) < EPS);
    REQUIRE( std::abs(withThreads - (-42)) < EPS);
    REQUIRE( std::abs(withThreads - noThreads) < EPS);
  }

  SECTION("Check sample 2") {
    thrd::Matrix<long double> M({
      {  25,  -3,  15, -10,   5 },
      {  -3,  48,  19,  68, -10 },
      {  15,  19,   7, -16,   5 },
      { -10,  68, -16,  10,  -8 },
      {   5, -10,   5,  -8,  40 }
    });
    auto noThreads = M.Determinant();
    auto withThreads = M.Determinant(THREADS_COUNT);
    REQUIRE( std::abs(noThreads - (-24401688)) < EPS);
    REQUIRE( std::abs(withThreads - (-24401688)) < EPS);
    REQUIRE( std::abs(withThreads - noThreads) < EPS);
  }

  SECTION("Check sample 3") {
    thrd::Matrix<long double> M({
      { 0, 0, 3, 0, 0 },
      { 0, 3, 0, 2, 2 },
      { 4, 0, 7, 0, 6 },
      { 0, 3, 0, 3, 0 },
      { 0, 1, 2, 0, 0 }
    });
    auto noThreads = M.Determinant();
    auto withThreads = M.Determinant(THREADS_COUNT);
    REQUIRE( std::abs(noThreads - ( 72)) < EPS);
    REQUIRE( std::abs(withThreads - ( 72)) < EPS);
    REQUIRE( std::abs(withThreads - noThreads) < EPS);
  }

  SECTION("Check sample 4") {
    thrd::Matrix<long double> M({
      { 0, 3, 0, 0, 1 },
      { 3, 0, 2, 2, 0 },
      { 0, 7, 0, 6, 2 },
      { 3, 0, 3, 0, 0 },
      { 1, 2, 0, 0, 4 }
    });
    auto noThreads = M.Determinant();
    auto withThreads = M.Determinant(THREADS_COUNT);
    REQUIRE( std::abs(noThreads - ( 174)) < EPS);
    REQUIRE( std::abs(withThreads - ( 174)) < EPS);
    REQUIRE( std::abs(withThreads - noThreads) < EPS);
  }

  SECTION("Check sample 5") {
    thrd::Matrix<long double> M({
      { 0, 0, 0, 0, 1 },
      { 0, 3, 2, 2, 0 },
      { 4, 0, 0, 6, 2 },
      { 0, 3, 3, 0, 0 },
      { 0, 1, 0, 0, 4 }
    });
    auto noThreads = M.Determinant();
    auto withThreads = M.Determinant(THREADS_COUNT);
    REQUIRE( std::abs(noThreads - ( 24)) < EPS);
    REQUIRE( std::abs(withThreads - ( 24)) < EPS);
    REQUIRE( std::abs(withThreads - noThreads) < EPS);
  }

  SECTION("Check sample 6") {
    thrd::Matrix<long double> M({
      { 0, 0, 3, 0, 1 },
      { 0, 3, 0, 2, 0 },
      { 4, 0, 7, 6, 2 },
      { 0, 3, 0, 0, 0 },
      { 0, 1, 2, 0, 4 }
    });
    auto noThreads = M.Determinant();
    auto withThreads = M.Determinant(THREADS_COUNT);
    REQUIRE( std::abs(noThreads - ( 240)) < EPS);
    REQUIRE( std::abs(withThreads - ( 240)) < EPS);
    REQUIRE( std::abs(withThreads - noThreads) < EPS);
  }

  SECTION("Check sample 7") {
    thrd::Matrix<long double> M({
      { 0, 3, 0, 0, 1 },
      { 0, 0, 2, 2, 0 },
      { 4, 7, 0, 6, 2 },
      { 0, 0, 3, 0, 0 },
      { 0, 2, 0, 0, 4 }
    });
    auto noThreads = M.Determinant();
    auto withThreads = M.Determinant(THREADS_COUNT);
    REQUIRE( std::abs(noThreads - (-240)) < EPS);
    REQUIRE( std::abs(withThreads - (-240)) < EPS);
    REQUIRE( std::abs(withThreads - noThreads) < EPS);
  }

  SECTION("Check sample 8") {
    thrd::Matrix<long double> M({
      { 0, 0, 3, 0, 1 },
      { 0, 3, 0, 2, 0 },
      { 4, 0, 7, 0, 2 },
      { 0, 3, 0, 3, 0 },
      { 0, 1, 2, 0, 4 }
    });
    auto noThreads = M.Determinant();
    auto withThreads = M.Determinant(THREADS_COUNT);
    REQUIRE( std::abs(noThreads - (-120)) < EPS);
    REQUIRE( std::abs(withThreads - (-120)) < EPS);
    REQUIRE( std::abs(withThreads - noThreads) < EPS);
  }

}

TEST_CASE("Advanced samples") {

  SECTION("Check det[Matrix] == 0 if rank[Matrix] < dim[Matrix]") {
    for (auto i = 2; i < 10; ++i) {
      REQUIRE(thrd::Matrix<long double>(i, 3).Determinant() == 0);
    }
  }

  SECTION("Check Threading == NoThreaing ") {
    auto M = thrd::RandomMatrix(10);
    REQUIRE(M.Determinant() == M.Determinant(THREADS_COUNT));
  }

}
