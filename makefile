

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

help:
	@echo "\nUseful make commands"
	@echo " - make install  :	builds and installs the static library and header files"
	@echo " - make uninstall:	remove the static library and the header files from the system"
	@echo

install: static-lib install-static-lib install-headers

static-lib:
	@mkdir -p lib
	@rm -f lib/*
	@gcc -c src/alloc.c src/string.c src/string_iter.c
	@mv *.o lib/
	@ar rcs lib/libss.a lib/alloc.o lib/string.o lib/string_iter.o
	@rm -f lib/*.o

install-static-lib: static-lib
	@echo "Copying static library libss.a to $(PREFIX)/lib/libss.a"
	@cp lib/libss.a $(PREFIX)/lib/
	@echo "Done."

install-headers:
	@echo "Copying ss_string.h header to $(PREFIX)/include/ss.h"
	@cp src/string.h $(PREFIX)/include/ss.h
	@echo "Header file installed."

test:
	@gcc tests/tests.c tests/utils.c src/alloc.c src/string.c src/string_iter.c -o tests/tests
	@./tests/tests
	@rm -f ./tests/tests

uninstall:
	@rm -f $(PREFIX)/include/ss.h
	@rm -f $(PREFIX)/lib/libss.a

clear:
	@rm -rf lib
	@rm -rf tests/tests