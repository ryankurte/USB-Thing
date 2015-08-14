// myobject.cc
#include "bindings.hpp"

#include "nan.h"

using namespace v8;

Persistent<Function> Bindings::constructor;
char className[] = "UsbThing";

Bindings::Bindings() {
	this->usbthing = USBThing();
}

Bindings::~Bindings() {
}

void Bindings::Init(Handle<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, className));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, className),
               tpl->GetFunction());
}

void Bindings::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Bindings(...)`
    int value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    Bindings* obj = new Bindings();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `Bindings(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void Bindings::PlusOne(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Bindings* obj = ObjectWrap::Unwrap<Bindings>(args.Holder());
  
  args.GetReturnValue().Set(Number::New(isolate, 1));
}
