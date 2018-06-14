#pragma once

#include <string>

#include <Windows.h>
#include <wincrypt.h>

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20180502 {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Serialkeymanager.com Web API and thus makes sure
 * the responses from the web API has not been tampered with. This signature
 * verifier depends on the Microsoft CryptoAPI.
 *
 * In order for this signature verifier to work the modulus and exponent
 * must be set using the set_modulus_base64() and set_exponent_base64()
 * methods.
 */
class SignatureVerifier_CryptoAPI
{
public:
  SignatureVerifier_CryptoAPI();
  ~SignatureVerifier_CryptoAPI();

  void set_modulus_base64(basic_Error & e, std::string const& modulus_base64);
  void set_exponent_base64(basic_Error & e, std::string const& exponent_base64) {}
  bool verify_message(basic_Error & e, std::string const& message, std::string const& signature_base64) const;
private:
  HCRYPTPROV hProv_;
  HCRYPTKEY hPubKey_;

  void init(basic_Error & e);
  void set_modulus_base64_(basic_Error & e, std::string const& modulus_base64);
};

} // namespace v20180502

using namespace ::cryptolens_io::v20180502;

} // namespace cryptolens_io
