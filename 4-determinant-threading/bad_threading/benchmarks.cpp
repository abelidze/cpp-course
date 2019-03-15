#define BENCHPRESS_CONFIG_MAIN

#include <chrono>
#include <thread>
#include <algorithm>

#include "benchpress.hpp"
#include "determinant.hpp"


BENCHMARK("No threading - Random 10x10 matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 1; i < ctx->num_iterations(); ++i) {
    thrd::RandomMatrix(10).Determinant(1);
  }
});
BENCHMARK("No threading - Random NxN matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 2; i <= ctx->num_iterations() && i < 12; ++i) {
    thrd::RandomMatrix(i).Determinant(1);
  }
});

BENCHMARK("2 Threads - Random 10x10 matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 1; i < ctx->num_iterations(); ++i) {
    thrd::RandomMatrix(10).Determinant(2);
  }
});
BENCHMARK("2 Threads - Random NxN matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 2; i <= ctx->num_iterations() && i < 12; ++i) {
    thrd::RandomMatrix(i).Determinant(2);
  }
});

BENCHMARK("3 Threads - Random 10x10 matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 1; i < ctx->num_iterations(); ++i) {
    thrd::RandomMatrix(10).Determinant(3);
  }
});
BENCHMARK("3 Threads - Random NxN matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 2; i <= ctx->num_iterations() && i < 12; ++i) {
    thrd::RandomMatrix(i).Determinant(3);
  }
});

BENCHMARK("4 Threads - Random 10x10 matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 1; i < ctx->num_iterations(); ++i) {
    thrd::RandomMatrix(10).Determinant(4);
  }
});
BENCHMARK("4 Threads - Random NxN matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 2; i <= ctx->num_iterations() && i < 12; ++i) {
    thrd::RandomMatrix(i).Determinant(4);
  }
});

BENCHMARK("5 Threads - Random 10x10 matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 1; i < ctx->num_iterations(); ++i) {
    thrd::RandomMatrix(10).Determinant(5);
  }
});
BENCHMARK("5 Threads - Random NxN matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 2; i <= ctx->num_iterations() && i < 12; ++i) {
    thrd::RandomMatrix(i).Determinant(5);
  }
});

BENCHMARK("6 Threads - Random 10x10 matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 1; i < ctx->num_iterations(); ++i) {
    thrd::RandomMatrix(10).Determinant(6);
  }
});
BENCHMARK("6 Threads - Random NxN matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 2; i <= ctx->num_iterations() && i < 12; ++i) {
    thrd::RandomMatrix(i).Determinant(6);
  }
});

BENCHMARK("7 Threads - Random 10x10 matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 1; i < ctx->num_iterations(); ++i) {
    thrd::RandomMatrix(10).Determinant(7);
  }
});
BENCHMARK("7 Threads - Random NxN matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 2; i <= ctx->num_iterations() && i < 12; ++i) {
    thrd::RandomMatrix(i).Determinant(7);
  }
});

BENCHMARK("8 Threads - Random 10x10 matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 1; i < ctx->num_iterations(); ++i) {
    thrd::RandomMatrix(10).Determinant(8);
  }
});
BENCHMARK("8 Threads - Random NxN matrix", [](benchpress::context* ctx) {
  ctx->reset_timer();
  for (size_t i = 2; i <= ctx->num_iterations() && i < 12; ++i) {
    thrd::RandomMatrix(i).Determinant(8);
  }
});
