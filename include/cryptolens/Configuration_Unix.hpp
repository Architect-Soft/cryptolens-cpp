#pragma once

#include "RequestHandler_curl.hpp"
#include "SignatureVerifier_OpenSSL.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename MachineCodeComputer_>
struct Configuration_Unix {
  using RequestHandler = RequestHandler_curl;
  using SignatureVerifier = SignatureVerifier_OpenSSL;
  using MachineCodeComputer = MachineCodeComputer_;
};

} // namespace v20190401

namespace latest {

template<typename MachineCodeComputer_>
using Configuration_Unix = ::cryptolens_io::v20190401::Configuration_Unix<MachineCodeComputer_>;

} // namespace latest

} // namespace cryptolens_io
