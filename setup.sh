

# Command to generate docs.
if [ "docs" = $1 ]; then
  rm -rf build
  cmake -S . -DSS_ALLOC_EXIT=ON -B build/
  cd build
  cmake --build .
  cd ..

  ./build/docs \
  "src/string.c" \
  "src/string_iter.c" \
  "src/string_fmt.c"
fi

# Command to perform tests.
if [ "test" = $1 ]; then
  rm -rf build
  cmake -S . -DSS_ALLOC_EXIT=ON -B build/
  cd build
  cmake --build .
  cd ..
  ./build/ss_tests
fi

# Command to install the library.
if [ "install" = $1 ]; then

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

  rm -rf build
  cmake -S . ${OPTIONS} -B build/
  cd build
  cmake --build .
  cmake --install .
  cd ..
fi

rm tmp/ss.h