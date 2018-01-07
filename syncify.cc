#include <nan.h>
#include <uv.h>

using namespace v8;

void Loop(const Nan::FunctionCallbackInfo<Value>& info) {
  auto isolate = info.GetIsolate();
  
  Local<Array> ret = Array::New(isolate, 2);
  info.GetReturnValue().Set(ret);

  if (!info[0]->IsPromise()) {
    ret->Set(0, Integer::New(isolate, Promise::kFulfilled));
    ret->Set(1, info[0]);
    return;
  }
  
  Local<Promise> promise = info[0].As<Promise>();

  uv_loop_t* loop = uv_default_loop();
  int state = promise->State();
  while (state == Promise::kPending) {
    uv_run(loop, UV_RUN_ONCE);
    isolate->RunMicrotasks();
    state = promise->State();
  }

  ret->Set(0, Integer::New(isolate, state));
  ret->Set(1, promise->Result());
}

void IsPromise(const Nan::FunctionCallbackInfo<Value>& info) {
  info.GetReturnValue().Set(info[0]->IsPromise());
}

void NanSetProperty(Local<Object> recv, const char* name, Local<Value> value) {
  recv->Set(Nan::New(name).ToLocalChecked(), value);
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Nan::SetMethod(exports, "loop", Loop);
  Nan::SetMethod(exports, "isPromise", IsPromise);
  NanSetProperty(exports, "kPending", Nan::New(v8::Promise::kPending));
  NanSetProperty(exports, "kFulfilled", Nan::New(v8::Promise::kFulfilled));
  NanSetProperty(exports, "kRejected", Nan::New(v8::Promise::kRejected));
}

NODE_MODULE(syncify, Init)
