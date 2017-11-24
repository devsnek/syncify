#include <uv.h>
#include <napi.h>

Napi::Value Run(const Napi::CallbackInfo& info) {
  uv_run(uv_default_loop(), UV_RUN_ONCE);
  return info.Env().Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "run"), Napi::Function::New(env, Run));
  return exports;
}

NODE_API_MODULE(uv_run_once, Init)
