{
  "targets": [
    {
      "target_name": "syncify",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ "syncify.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('nan')\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
