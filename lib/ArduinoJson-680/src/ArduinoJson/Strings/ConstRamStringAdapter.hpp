// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // size_t
#include <string.h>  // strcmp

namespace ARDUINOJSON_NAMESPACE {

class ConstRamStringAdapter {
 public:
  ConstRamStringAdapter(const char* str = 0) : _str(str) {}

  bool equals(const char* expected) const {
    const char* actual = _str;
    if (!actual || !expected) return actual == expected;
    return strcmp(actual, expected) == 0;
  }

  bool isNull() const {
    return !_str;
  }

  template <typename TMemoryPool>
  char* save(TMemoryPool*) const {
    return 0;
  }

  size_t size() const {
    return strlen(_str);
  }

  const char* data() const {
    return _str;
  }

  bool isStatic() const {
    return true;
  }

 protected:
  const char* _str;
};

inline ConstRamStringAdapter adaptString(const char* str) {
  return ConstRamStringAdapter(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
