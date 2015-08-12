#include <nan.h>
#include "usbthing.cpp"

NAN_METHOD(anArray) {
    NanScope();
    v8::Local<v8::Array> arr = NanNew<v8::Array>(3);
    arr->Set(0, NanNew<v8::Number>(1));
    arr->Set(1, NanNew<v8::Number>(2));
    arr->Set(2, NanNew<v8::Number>(3));
    NanReturnValue(arr);
}