#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <unordered_map>

#include "optional.hpp"

#include "../ArduinoJson.hpp"

#include "ActivateError.hpp"
#include "basic_Error.hpp"
#include "RawLicenseKey.hpp"
#include "LicenseKey.hpp"
#include "LicenseKeyInformation.hpp"
#include "LicenseKeyChecker.hpp"
#include "api.hpp"

namespace cryptolens_io {

namespace v20180502 {

namespace internal {

template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( basic_Error & e
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  );

} // namespace internal

template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate_raw
  ( basic_Error & e
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  if (e) { return nullopt; }

  auto x = internal::handle_activate(e, signature_verifier, response);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_HANDLE_ACTIVATE_RAW); }
  return x;
}

template<typename SignatureVerifier>
RawLicenseKey
handle_activate_raw_exn
  ( experimental_v1 experimental
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  basic_Error e;
  optional<RawLicenseKey> raw_license_key =
    handle_activate(e, signature_verifier, response);

  if (raw_license_key && !e) { return *raw_license_key; }

  throw ActivateError::from_server_response(NULL);
}

template<typename SignatureVerifier>
optional<LicenseKey>
handle_activate
  ( basic_Error & e
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  if (e) { return nullopt; }

  optional<RawLicenseKey> x = internal::handle_activate(e, signature_verifier, response);
  optional<LicenseKeyInformation> y = LicenseKeyInformation::make(e, x);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_HANDLE_ACTIVATE); }

  return LicenseKey(std::move(*y), std::move(*x));
}

/**
 * This class makes it possible to interact with the SKM Web API. Among the
 * various methods available in the Web API the only ones currently supported
 * in the C++ API are Activate and Deactivate.
 *
 * This class uses two policy classes, SignatureVerifier and RequestHandler,
 * which are responsible for handling verification of signatures and making
 * requests to the Web API, respectivly. Consult the documentation for the
 * chosen policy classes since in some cases special initialization may be
 * neccessary.
 */
template<typename RequestHandler, typename SignatureVerifier>
class basic_SKM
{
public:
  basic_SKM() { }

  optional<LicenseKey>
  activate
    ( basic_Error & e
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );

  optional<RawLicenseKey>
  activate_raw
    ( basic_Error & e
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );

  RawLicenseKey
  activate_raw_exn
    ( experimental_v1 experimental
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );

  SignatureVerifier signature_verifier;
  RequestHandler request_handler;

private:
  optional<RawLicenseKey>
  activate_
    ( basic_Error & e
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );
};

/**
 * Make an Activate request to the SKM Web API
 *
 * Arguments:
 *   token - acces token to use
 *   product_id - the product id
 *   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
 *   machine_code - the machine code, i.e. a string that identifies a device
 *                  for activation.
 *
 * Returns:
 *   An optional with a RawLicenseKey representing if the request was
 *   successful or not.
 */
template<typename RequestHandler, typename SignatureVerifier>
optional<LicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::activate
  ( basic_Error & e
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  optional<RawLicenseKey> x = this->activate_
      ( e
      , std::move(token)
      , std::move(product_id)
      , std::move(key)
      , std::move(machine_code)
      , fields_to_return
      );
  optional<LicenseKeyInformation> y = LicenseKeyInformation::make(e, x);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_ACTIVATE); return nullopt; }
  return LicenseKey(std::move(*y), std::move(*x));
}

/**
 * Make an Activate request to the SKM Web API
 *
 * Arguments:
 *   token - acces token to use
 *   product_id - the product id
 *   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
 *   machine_code - the machine code, i.e. a string that identifies a device
 *                  for activation.
 *
 * Returns:
 *   An optional with a RawLicenseKey representing if the request was
 *   successful or not.
 */
template<typename RequestHandler, typename SignatureVerifier>
optional<RawLicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::activate_raw
  ( basic_Error & e
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  auto x = this->activate_( e
                          , std::move(token)
                          , std::move(product_id)
                          , std::move(key)
                          , std::move(machine_code)
                          , fields_to_return
                          );
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_ACTIVATE_RAW); }
  return x;
}

template<typename RequestHandler, typename SignatureVerifier>
optional<RawLicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::activate_
  ( basic_Error & e
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  std::unordered_map<std::string,std::string> args;
  args["token"] = token;
  args["ProductId"] = product_id;
  args["Key"] = key;
  args["Sign"] = "true";
  args["MachineCode"] = machine_code;
  // Fix since to_string is not available everywhere
  //args["FieldsToReturn"] = std::to_string(fields_to_return);
  std::ostringstream stm; stm << fields_to_return;
  args["FieldsToReturn"] = stm.str();
  args["SignMethod"] = "1";
  args["v"] = "1";

  std::string response = request_handler.make_request(e, "Activate", args);

  return handle_activate_raw(e, this->signature_verifier, response);
}

/**
 * Make an Activate request to the SKM Web API
 *
 * Arguments:
 *   token - acces token to use
 *   product_id - the product id
 *   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
 *   machine_code - the machine code, i.e. a string that identifies a device
 *                  for activation.
 *
 * Returns:
 *   A RawLicenseKey. If the request is unsuecessful an ActivateError is thrown.
 */
template<typename RequestHandler, typename SignatureVerifier>
RawLicenseKey
basic_SKM<RequestHandler, SignatureVerifier>::activate_raw_exn
  ( experimental_v1 experimental
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  basic_Error e;
  optional<RawLicenseKey> raw_license_key =
    activate_raw( e, std::move(token), std::move(product_id), std::move(key)
            , std::move(machine_code), fields_to_return);

  if (!e) { return *raw_license_key; }
  throw ActivateError::from_server_response(NULL);
}

namespace internal {

int
activate_parse_server_error_message(char const* server_response);

template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( basic_Error & e
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  if (e) { return nullopt; }

  using namespace errors;
  api::main api;

  using namespace ArduinoJson;
  DynamicJsonBuffer jsonBuffer;
  JsonObject & j = jsonBuffer.parseObject(response);

  if (!j.success()) { e.set(api, Subsystem::Json); return nullopt; }

  if (!j["result"].is<int>() || j["result"].as<int>() != 0) {
    if (!j["message"].is<const char*>() || j["message"].as<char const*>() == NULL) {
      e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
      return nullopt;
    }

    int reason = activate_parse_server_error_message(j["message"].as<char const*>());
    e.set(api, Subsystem::Main, reason);
    return nullopt;
  }

  if (!j["licenseKey"].is<char const*>() || j["licenseKey"].as<char const*>() == NULL) {
    e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
    return nullopt;
  }

  if (!j["signature"].is<char const*>() || j["signature"].as<char const*>() == NULL) {
    e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
    return nullopt;
  }

  return RawLicenseKey::make
           ( e
           , signature_verifier
           , j["licenseKey"].as<char const*>()
           , j["signature"].as<char const*>()
	   );
}

} // namespace internal

} // namespace v20180502

using namespace ::cryptolens_io::v20180502;

} // namespace cryptolens_io
