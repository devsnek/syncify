#include "napi.h"
#include <v8.h>
#include <uv.h>

using namespace Napi;

FunctionReference tickCallback;

void SetTickCallback(const CallbackInfo& args) {
  tickCallback = Persistent(args[0].As<Function>());
}

static napi_value JsValueFromV8LocalValue(v8::Local<v8::Value> local) {
  return reinterpret_cast<napi_value>(*local);
}

static v8::Local<v8::Value> V8LocalValueFromJsValue(napi_value v) {
  v8::Local<v8::Value> local;
  memcpy(&local, &v, sizeof(v));
  return local;
}

static Value Loop(const CallbackInfo& args) {
  Env env = args.Env();

  Array ret = Array::New(env, 2);
  int32_t state_index = 0;
  int32_t result_index = 1;

  if (!args[0].IsPromise()) {
    ret[state_index] = Number::New(env, v8::Promise::kFulfilled);
    ret[result_index] = args[0];
    return ret;
  }

  v8::Local<v8::Promise> promise =
    V8LocalValueFromJsValue(args[0]).As<v8::Promise>();

  v8::Isolate* isolate = v8::Isolate::GetCurrent();

  uv_loop_t* loop = uv_default_loop();
  int state = promise->State();
  while (state == v8::Promise::kPending) {
    uv_run(loop, UV_RUN_ONCE);
    isolate->RunMicrotasks();
    tickCallback({});
    state = promise->State();
  }

  ret[state_index] = Number::New(env, state);
  ret[result_index] = JsValueFromV8LocalValue(promise->Result());

  return ret;
}

static Value IsPromise(const CallbackInfo& args) {
  return Boolean::New(args.Env(), args[0].IsPromise());
}

static Object Init(Env env, Object exports) {
  exports["loop"] = Function::New(env, Loop);
  exports["setTickCallback"] = Function::New(env, SetTickCallback);
  exports["isPromise"] = Function::New(env, IsPromise);

#define V(name) \
  exports[#name] = Number::New(env, v8::Promise::name);
  V(kPending);
  V(kFulfilled);
  V(kRejected);
#undef V

  return exports;
}

NODE_API_MODULE(syncify, Init);
