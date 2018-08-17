# librlwe

**librlwe** is a fast, lightweight, and easy-to-use library for doing ring-learning with errors (RLWE) cryptography.

Specifically, librlwe implements the Fan-Vercauteren fully homomorphic cryptosystem, which is based off of the RLWE problem. 
It contains support for RLWE-based encryption, decryption, key exchange, signing, and homomorphic addition & multiplication.
Note that it does its best to stick to the cryptosystem outlined in the FV paper;
however, relinearization is not immediately performed after every homomorphic multiplication and is instead
separated into its own function. This means that the ciphertexts can grow in size without bound until the user decides
that it is necessary to relinearize it.

For anyone without significant background on RLWE, I would recommend checking out these links:
* [Somewhat Practical Fully Homomorphic Encryption](https://eprint.iacr.org/2012/144.pdf) - Junfeng Fan and Frederik Vercauteren's original paper on FV-style FHE
* [Homomorphic Encryption from RLWE](https://cryptosith.org/michael/data/talks/2012-01-10-MSR-Cambridge.pdf) - presentation given at MSR Cambridge
* [Microsoft's SEAL Library Manual](https://www.microsoft.com/en-us/research/wp-content/uploads/2017/12/sealmanual.pdf) - of which this is based off of
* [N1 Analytic's Blog on Homomorphic Encryption using RLWE](https://blog.n1analytics.com/homomorphic-encryption-illustrated-primer/)
* [Parameters for RLWE Cryptography](http://www.ringlwe.info/parameters-for-rlwe.html)

However, it is possible to still use this library without in-depth knowledge of how the RLWE cryptographic system works.
This is because most of the algorithmic details have been abstracted away.

Here's an example of what you can do with librlwe:

```c++
// Set up some parameters for the RLWE algorithm
rlwe::KeyParameters params(1024, 9214347247561474048, 290764801);

// Generate a keypair
rlwe::PrivateKey priv = params.GeneratePrivateKey();
rlwe::PublicKey pub = params.GeneratePublicKey();

// Encode some plaintext integer as a polynomial in the plaintext ring
// The coefficients of the polynomial are equal to the binary representation of the integer
rlwe::Plaintext encoded_plaintext = params.EncodeInteger(1337);

// Encrypt the plaintext using the public key 
rlwe::Ciphertext ciphertext = pub.Encrypt(encoded_plaintext);

[...]

// Decrypt the plaintext using the private key
rlwe::Plaintext decrypted_plaintext = priv.Decrypt(ciphertext);

// Prints "1337"
std::cout << params.DecodeInteger(decrypted_plaintext) << std::endl;
```

## Installation

It's recommended you do an out-of-source build & install.

After cloning into this repository, run:

```
mkdir build && cd build
cmake ..
make
make install
```

## Implementation Details

Internally, librlwe uses [NTL](http://www.shoup.net/ntl/) for doing fast polynomial arithmetic. 
All keys, plaintexts, and ciphertexts store their polynomials as `NTL:ZZX` objects.
However, these polynomials are in the ring `Z_q/(f)` where `f` is a cyclotomic polynomial of the form `x^n + 1`.
Whenever operations are performed on them, they are usually converted to `NTL::ZZ_pX` and a temporary modulus is pushed until the operation completes.
