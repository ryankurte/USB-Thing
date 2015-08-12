{
  "targets": [
    {
      "target_name": "binding",
      "include_dirs": [ "include", "../common/include", "<!(node -e \"require('nan')\")" ],
      "sources": [ "source/usbthing.cpp", "source/usbthing.c"  ]
    }
  ]
}