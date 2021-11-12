
# List of options to be chosen.
WITH_EXIT_OPTION="--with-exit"

OPTIONS=""
for var in "$@"
do
    if [ $var = $WITH_EXIT_OPTION ]; then
      echo "${WITH_EXIT_OPTION} option selected"
      OPTIONS="${OPTIONS} -DSS_ALLOC_EXIT=ON"
    fi
done

# Build the project with CMake, perform tests and install the project.
rm -rf build
cmake -S . ${OPTIONS} -B build/
cd build
cmake --build .
./ss_tests
cmake --install .
cd ..