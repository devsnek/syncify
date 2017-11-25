#include <nan.h>
#include <uv.h>

Nan::Callback tickCallback;

void SetTickCallback(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  tickCallback.SetFunction(info[0].As<v8::Function>());
}

void Loop(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  auto isolate = info.GetIsolate();
  
  v8::Local<v8::Array> ret = v8::Array::New(isolate, 2);
  info.GetReturnValue().Set(ret);

  if (!info[0]->IsPromise()) {
    ret->Set(0, v8::Integer::New(isolate, v8::Promise::kFulfilled));
    ret->Set(1, info[0]);
    return;
  }
  
  v8::Local<v8::Promise> promise = info[0].As<v8::Promise>();

  uv_loop_t* loop = uv_default_loop();
  int state = promise->State();
  while (state == v8::Promise::kPending) {
    uv_run(loop, UV_RUN_ONCE);
    tickCallback.Call(0, 0);
    state = promise->State();
  }

  ret->Set(0, v8::Integer::New(isolate, state));
  ret->Set(1, promise->Result());
}

void IsPromise(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(info[0]->IsPromise());
}

void NanSetProperty(v8::Local<v8::Object> recv, const char* name, v8::Local<v8::Value> value) {
  recv->Set(Nan::New(name).ToLocalChecked(), value);
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Nan::SetMethod(exports, "setTickCallback", SetTickCallback);
  Nan::SetMethod(exports, "loop", Loop);
  Nan::SetMethod(exports, "isPromise", IsPromise);
  NanSetProperty(exports, "kPending", Nan::New(v8::Promise::kPending));
  NanSetProperty(exports, "kFulfilled", Nan::New(v8::Promise::kFulfilled));
  NanSetProperty(exports, "kRejected", Nan::New(v8::Promise::kRejected));
}

NODE_MODULE(syncify, Init)
