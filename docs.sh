
# Build the project with CMake, perform tests and install the project.
rm -rf build
cmake -S . -DSS_ALLOC_EXIT=ON -B build/
cd build
cmake --build .
cd ..

./build/docs \
  "src/string.c" \
  "src/string_iter.c" \
  "src/string_fmt.c"
