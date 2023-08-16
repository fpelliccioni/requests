# conan.lock creation

```
rm conan.lock
conan lock create conanfile.py --version="1.0.0" --update
```

# Build

```
rm -rf conanbuild
conan lock create conanfile.py --version "1.0.0" --lockfile=conan.lock --lockfile-out=conanbuild/conan.lock
conan install conanfile.py --lockfile=conanbuild/conan.lock -of conanbuild --build=missing

cmake --preset conan-release -DBOOST_REQUESTS_BUILD_TESTS=ON
cmake --build --preset conan-release -j4
```