// addon.cc
#include <nan.h>
#include "bindings.hpp"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Bindings::Init(exports);
}

NODE_MODULE(addon, InitAll)