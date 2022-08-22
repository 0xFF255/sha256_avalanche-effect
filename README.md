# SHA256 avalance effect
To compile, you need OpenSSL installed on your system. Then you can just run
```bash
mkdir build && cd build
CXX=clang++ cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
ninja
./avalance
```
