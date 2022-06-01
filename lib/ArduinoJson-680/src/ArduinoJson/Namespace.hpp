// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "version.hpp"

#include "Configuration.hpp"

#define ARDUINOJSON_DO_CONCAT(A, B) A##B
#define ARDUINOJSON_CONCAT2(A, B) ARDUINOJSON_DO_CONCAT(A, B)
#define ARDUINOJSON_CONCAT4(A, B, C, D) \
  ARDUINOJSON_CONCAT2(ARDUINOJSON_CONCAT2(A, B), ARDUINOJSON_CONCAT2(C, D))
#define ARDUINOJSON_CONCAT8(A, B, C, D, E, F, G, H)    \
  ARDUINOJSON_CONCAT2(ARDUINOJSON_CONCAT4(A, B, C, D), \
                      ARDUINOJSON_CONCAT4(E, F, G, H))

#define ARDUINOJSON_NAMESPACE                                                  \
  ARDUINOJSON_CONCAT8(ArduinoJson, ARDUINOJSON_VERSION_MAJOR,                  \
                      ARDUINOJSON_VERSION_MINOR, ARDUINOJSON_VERSION_REVISION, \
                      _, ARDUINOJSON_USE_LONG_LONG, _, ARDUINOJSON_USE_DOUBLE)
