#include <node.h>
#include <v8.h>
#include <uv.h>

using namespace v8;

template <typename T> inline void USE(T&&) {};

Global<Function> tickCallback;

void SetTickCallback(const FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  tickCallback.Reset(isolate, info[0].As<Function>());
}

void Loop(const FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  Local<Array> ret = Array::New(isolate, 2);
  info.GetReturnValue().Set(ret);

  if (!info[0]->IsPromise()) {
    USE(ret->Set(context, 0, Integer::New(isolate, Promise::kFulfilled)));
    USE(ret->Set(context, 1, info[0]));
    return;
  }

  Local<Promise> promise = info[0].As<Promise>();
  Local<Function> cb = tickCallback.Get(isolate);
  Local<Value> undefined = Undefined(isolate);
  Local<Value> args[] = {};

  uv_loop_t* loop = uv_default_loop();
  int state = promise->State();
  while (state == Promise::kPending) {
    uv_run(loop, UV_RUN_ONCE);
    isolate->RunMicrotasks();
    USE(cb->Call(context, undefined, 0, args));
    state = promise->State();
  }

  USE(ret->Set(context, 0, Integer::New(isolate, state)));
  USE(ret->Set(context, 1, promise->Result()));
}

void IsPromise(const FunctionCallbackInfo<Value>& info) {
  info.GetReturnValue().Set(info[0]->IsPromise());
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Isolate* isolate = Isolate::GetCurrent();
  Local<Context> context = isolate->GetCurrentContext();

#define V(name, fn) \
  USE(exports->Set(context, String::NewFromUtf8(isolate, name), FunctionTemplate::New(isolate, fn)->GetFunction()))
  V("loop", Loop);
  V("setTickCallback", SetTickCallback);
  V("isPromise", IsPromise);
#undef V

#define V(name) \
  USE(exports->Set(context, String::NewFromUtf8(isolate, #name), Integer::New(isolate, v8::Promise::name)))
  V(kPending);
  V(kFulfilled);
  V(kRejected);
#undef V
}

NODE_MODULE(syncify, Init)
