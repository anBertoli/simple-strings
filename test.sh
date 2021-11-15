
# Build the project with CMake, perform tests and install the project.
rm -rf build
cmake -S . -DSS_ALLOC_EXIT=ON -B build/
cd build
cmake --build .
cd ..

./build/ss_tests