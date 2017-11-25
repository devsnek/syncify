#include <nan.h>
#include <uv.h>

void Loop(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  auto isolate = info.GetIsolate();
  
  v8::Local<v8::Array> ret = v8::Array::New(isolate, 2);

  if (!info[0]->IsPromise()) {
    info.GetReturnValue().Set(ret);
    return;
  }
  
  v8::Local<v8::Promise> promise = info[0].As<v8::Promise>();
  Nan::Callback tickCallback(info[1].As<v8::Function>());

  int state = promise->State();
  while (state == v8::Promise::kPending) {
    uv_run(uv_default_loop(), UV_RUN_ONCE);
    tickCallback.Call(0, 0);
    state = promise->State();
  }

  ret->Set(0, v8::Integer::New(isolate, state));
  ret->Set(1, promise->Result());

  info.GetReturnValue().Set(ret);
}

void IsPromise(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(info[0]->IsPromise());
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Nan::SetMethod(exports, "loop", Loop);
  Nan::SetMethod(exports, "isPromise", IsPromise);
  exports->Set(Nan::New("kPending").ToLocalChecked(), Nan::New(v8::Promise::kPending));
  exports->Set(Nan::New("kFulfilled").ToLocalChecked(), Nan::New(v8::Promise::kFulfilled));
  exports->Set(Nan::New("kRejected").ToLocalChecked(), Nan::New(v8::Promise::kRejected));
}

NODE_MODULE(syncify, Init)
