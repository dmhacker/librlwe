#include "fv.h"
#include "sample.h"

#include <cassert>

using namespace rlwe::fv;

KeyParameters::KeyParameters(long n, ZZ q, ZZ t, long log_w, float sigma) : 
  n(n), q(q), t(t), log_w(log_w), sigma(sigma),
  delta(q / t), downscale(conv<RR>(t) / conv<RR>(q))
{
  // Assert that n is even, assume that it is a power of 2
  assert(n % 2 == 0);

  // Doesn't matter what this is, since the max coefficient is 1 for the cyclotomic polynomial
  ZZ_pPush push;
  ZZ_p::init(q);

  // Create a cyclotomic polynomial that serves as the modulus for the ring
  ZZ_pX cyclotomic;

  // The cyclotomic polynomial is x^n + 1 
  SetCoeff(cyclotomic, n, 1);
  SetCoeff(cyclotomic, 0, 1);

  // Build the modulus using the cyclotomic polynomial representation
  build(phi, cyclotomic);

  // Calculate decomposition base and mask
  power2(w, log_w);
  w_mask = w - 1; 
  l = floor(log(q) / log(w));

  // Generate probability matrix
  pmat_rows = sigma * PROBABILITY_MATRIX_BOUNDS_SCALAR;
  pmat = KnuthYaoGaussianMatrix(pmat_rows, sigma); 
}