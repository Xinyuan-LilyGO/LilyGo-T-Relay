// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Array/ArrayRef.hpp"
#include "ObjectRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TObject>
template <typename TString>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(
    const TString& key) const {
  return impl()->getOrCreate(key).template to<ArrayRef>();
}

template <typename TObject>
template <typename TChar>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(TChar* key) const {
  return impl()->getOrCreate(key).template to<ArrayRef>();
}

template <typename TObject>
template <typename TString>
inline ObjectRef ObjectShortcuts<TObject>::createNestedObject(
    const TString& key) const {
  return impl()->getOrCreate(key).template to<ObjectRef>();
}

template <typename TObject>
template <typename TChar>
inline ObjectRef ObjectShortcuts<TObject>::createNestedObject(
    TChar* key) const {
  return impl()->getOrCreate(key).template to<ObjectRef>();
}
}  // namespace ARDUINOJSON_NAMESPACE
