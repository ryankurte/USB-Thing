// myobject.h
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <nan.h>
#include "usbthing.hpp"

class Bindings : public Nan::ObjectWrap {
public:
	static void Init(v8::Handle<v8::Object> exports);

private:
	Bindings();
	~Bindings();

	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Persistent<v8::Function> constructor;
	USBThing usbthing;
};

#endif