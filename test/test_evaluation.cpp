#include "catch.hpp"
#include "../src/rlwe.hpp"

TEST_CASE("FV-style homomorphic addition") {
  // Set up parameters
  rlwe::KeyParameters params(16, 874, 7);  

  // Compute keys
  rlwe::PrivateKey priv = params.GeneratePrivateKey();
  rlwe::PublicKey pub = params.GeneratePublicKey(priv);

  // Generate two random plaintexts
  rlwe::Plaintext pt1(rlwe::random::UniformSample(params.GetPolyModulusDegree(), params.GetPlainModulus()), params);
  rlwe::Plaintext pt2(rlwe::random::UniformSample(params.GetPolyModulusDegree(), params.GetPlainModulus()), params);

  // Convert both to ciphertexts 
  rlwe::Ciphertext ct1 = pub.Encrypt(pt1);
  rlwe::Ciphertext ct2 = pub.Encrypt(pt2);

  // Perform homomorphic addition
  rlwe::Ciphertext ct = ct1.Add(ct2);

  // Decrypt resultant ciphertext
  rlwe::Plaintext pt = priv.Decrypt(ct);

  // Compute the plaintext additions separately
  ZZ_pPush push;
  ZZ_p::init(params.GetPlainModulus());
  ZZ_pX m_p = conv<ZZ_pX>(pt1.GetM()) + conv<ZZ_pX>(pt2.GetM());
  ZZX m = conv<ZZX>(m_p);

  REQUIRE(pt.GetM() == m);
}
