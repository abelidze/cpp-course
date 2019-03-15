#include <cmath>
#include <thread>

// #include <iostream>
// #include <iomanip>

#include "catch.hpp"
#include "determinant.hpp"
#include "samples.hpp"

const int THREADS_COUNT = std::thread::hardware_concurrency();
const long double EPS = 1e-13;

TEST_CASE("Predefined samples") {

  SECTION("CHECK for A no threading") {
    REQUIRE( std::fabs(sample::A.matrix.DeterminantLaplace() - sample::A.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::A.matrix.DeterminantLU() - sample::A.expectedDet) < EPS);
  }
  SECTION("CHECK for A with threading") {
    REQUIRE( std::fabs(sample::A.matrix.DeterminantLaplace(THREADS_COUNT) - sample::A.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::A.matrix.DeterminantLU(THREADS_COUNT) - sample::A.expectedDet) < EPS);
  }
  SECTION("CHECK for B no threading") {
    REQUIRE( std::fabs(sample::B.matrix.DeterminantLaplace() - sample::B.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::B.matrix.DeterminantLU() - sample::B.expectedDet) < EPS);
  }
  SECTION("CHECK for B with threading") {
    REQUIRE( std::fabs(sample::B.matrix.DeterminantLaplace(THREADS_COUNT) - sample::B.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::B.matrix.DeterminantLU(THREADS_COUNT) - sample::B.expectedDet) < EPS);
  }
  SECTION("CHECK for C no threading") {
    REQUIRE( std::fabs(sample::C.matrix.DeterminantLaplace() - sample::C.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::C.matrix.DeterminantLU() - sample::C.expectedDet) < EPS);
  }
  SECTION("CHECK for C with threading") {
    REQUIRE( std::fabs(sample::C.matrix.DeterminantLaplace(THREADS_COUNT) - sample::C.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::C.matrix.DeterminantLU(THREADS_COUNT) - sample::C.expectedDet) < EPS);
  }
  SECTION("CHECK for D no threading") {
    REQUIRE( std::fabs(sample::D.matrix.DeterminantLaplace() - sample::D.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::D.matrix.DeterminantLU() - sample::D.expectedDet) < EPS);
  }
  SECTION("CHECK for D with threading") {
    REQUIRE( std::fabs(sample::D.matrix.DeterminantLaplace(THREADS_COUNT) - sample::D.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::D.matrix.DeterminantLU(THREADS_COUNT) - sample::D.expectedDet) < EPS);
  }
  SECTION("CHECK for E no threading") {
    REQUIRE( std::fabs(sample::E.matrix.DeterminantLaplace() - sample::E.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::E.matrix.DeterminantLU() - sample::E.expectedDet) < EPS);
  }
  SECTION("CHECK for E with threading") {
    REQUIRE( std::fabs(sample::E.matrix.DeterminantLaplace(THREADS_COUNT) - sample::E.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::E.matrix.DeterminantLU(THREADS_COUNT) - sample::E.expectedDet) < EPS);
  }
  SECTION("CHECK for F no threading") {
    REQUIRE( std::fabs(sample::F.matrix.DeterminantLaplace() - sample::F.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::F.matrix.DeterminantLU() - sample::F.expectedDet) < EPS);
  }
  SECTION("CHECK for F with threading") {
    REQUIRE( std::fabs(sample::F.matrix.DeterminantLaplace(THREADS_COUNT) - sample::F.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::F.matrix.DeterminantLU(THREADS_COUNT) - sample::F.expectedDet) < EPS);
  }
  SECTION("CHECK for G no threading") {
    REQUIRE( std::fabs(sample::G.matrix.DeterminantLaplace() - sample::G.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::G.matrix.DeterminantLU() - sample::G.expectedDet) < EPS);
  }
  SECTION("CHECK for G with threading") {
    REQUIRE( std::fabs(sample::G.matrix.DeterminantLaplace(THREADS_COUNT) - sample::G.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::G.matrix.DeterminantLU(THREADS_COUNT) - sample::G.expectedDet) < EPS);
  }
  SECTION("CHECK for H no threading") {
    REQUIRE( std::fabs(sample::H.matrix.DeterminantLaplace() - sample::H.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::H.matrix.DeterminantLU() - sample::H.expectedDet) < EPS);
  }
  SECTION("CHECK for H with threading") {
    REQUIRE( std::fabs(sample::H.matrix.DeterminantLaplace(THREADS_COUNT) - sample::H.expectedDet) < EPS);
    REQUIRE( std::fabs(sample::H.matrix.DeterminantLU(THREADS_COUNT) - sample::H.expectedDet) < EPS);
  }
  SECTION("CHECK for Hilb8 no threading") {
    REQUIRE( std::fabs(sample::Hilb8.matrix.DeterminantLaplace() - sample::Hilb8.expectedDet) < 1e-32 );
    REQUIRE( std::fabs(sample::Hilb8.matrix.DeterminantLU() - sample::Hilb8.expectedDet) < 1e-33 );
  }
  SECTION("CHECK for Hilb8 with threading") {
    REQUIRE( std::fabs(sample::Hilb8.matrix.DeterminantLaplace(THREADS_COUNT) == sample::Hilb8.expectedDet) < 1e-32 );
    REQUIRE( std::fabs(sample::Hilb8.matrix.DeterminantLU(THREADS_COUNT) == sample::Hilb8.expectedDet) < 1e-33 );
  }

}

TEST_CASE("Samples with diagonal dominating") {

  SECTION("CHECK DiagonalMatrix no threading") {
    REQUIRE(sample::DiagonalMatrix(5, 2).DeterminantLaplace() == std::pow(2, 5));
    REQUIRE(sample::DiagonalMatrix(5, 2).DeterminantLU() == std::pow(2, 5));
  }
  SECTION("CHECK DiagonalMatrix with threading") {
    REQUIRE(sample::DiagonalMatrix(5, 2).DeterminantLaplace(THREADS_COUNT) == std::pow(2, 5));
    REQUIRE(sample::DiagonalMatrix(5, 2).DeterminantLU(THREADS_COUNT) == std::pow(2, 5));
  }
  SECTION("CHECK TriangleMatrix no threading") {
    REQUIRE(sample::TriangleMatrix(10, 3).DeterminantLaplace() == std::pow(3, 10));
    REQUIRE(sample::TriangleMatrix(10, 3).DeterminantLU() == std::pow(3, 10));
  }
  SECTION("CHECK TriangleMatrix with threading") {
    REQUIRE(sample::TriangleMatrix(10, 3).DeterminantLaplace(THREADS_COUNT) == std::pow(3, 10));
    REQUIRE(sample::TriangleMatrix(10, 3).DeterminantLU(THREADS_COUNT) == std::pow(3, 10));
  }

}

TEST_CASE("Advanced tests") {

  SECTION("CHECK det[Matrix] == 0 if rank[Matrix] < dim[Matrix]") {
    for (auto i = 2; i < 10; ++i) {
      REQUIRE(thrd::Matrix<sample::value_t>(i, 3).DeterminantLaplace() == 0);
      REQUIRE(thrd::Matrix<sample::value_t>(i, 3).DeterminantLU() == 0);
    }
  }

  SECTION("CHECK RandomMatrixThreading == RandomMatrixNoThreaing ") {
    auto M = sample::RandomMatrix(10);
    REQUIRE(M.DeterminantLaplace() == M.Determinant(THREADS_COUNT, M.LAPLACE));
    REQUIRE(M.DeterminantLU() == M.Determinant(THREADS_COUNT, M.LU));
  }

  SECTION("CHECK Hilbert12MatrixThreading == Hilbert12MatrixNoThreaing ") {
    auto M = sample::Hilbert(12);
    auto det = M.Determinant();
    REQUIRE( std::fabs(det - sample::expectedHilbert(12)) < 1e-78 );
    REQUIRE( det == M.Determinant(THREADS_COUNT));
  }

}
