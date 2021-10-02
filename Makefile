
ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

help:
	@echo "\nUseful make commands"
	@echo " - make install  				builds and installs the static library and header files"
	@echo " - make install-with-exit  		builds and installs the static library and header files with the exit option"
	@echo " - make uninstall				remove the static library and the header files from the system"
	@echo

install: install-static-lib install-headers
install-with-exit: install-static-lib-exit install-headers

static-lib:
	@mkdir -p lib
	@rm -f lib/*
	@gcc -c src/alloc.c src/string.c src/string_iter.c src/string_fmt.c src/internal/debug.c
	@mv *.o lib/
	@ar rcs lib/libss.a lib/alloc.o lib/string.o lib/string_iter.o lib/string_fmt.o lib/debug.o
	@rm -f lib/*.o

static-lib-exit:
	@mkdir -p lib
	@rm -f lib/*
	@gcc -DSS_ALLOC_EXIT src/alloc.c src/string.c src/string_iter.c src/string_fmt.c src/internal/debug.c -c
	@mv *.o lib/
	@ar rcs lib/libss.a lib/alloc.o lib/string.o lib/string_iter.o lib/string_fmt.o lib/debug.o
	@rm -f lib/*.o

install-static-lib: static-lib
	@cp lib/libss.a $(PREFIX)/lib/
	@echo "Copied static library libss.a to $(PREFIX)/lib/libss.a"

install-static-lib-exit: static-lib-exit
	@cp lib/libss.a $(PREFIX)/lib/
	@echo "Copied static library libss.a to $(PREFIX)/lib/libss.a"

install-headers:
	@cp src/string.h $(PREFIX)/include/ss.h
	@echo "Copied ss_string.h header to $(PREFIX)/include/ss.h"

test:
	@gcc -DSS_ALLOC_EXIT \
		tests/tests.c \
		tests/utils.c \
		tests/framework/framework.c \
		src/alloc.c \
		src/string.c \
		src/string_iter.c \
		src/string_fmt.c \
		src/internal/debug.c \
		-o tests/tests

	@./tests/tests
	@rm -f ./tests/tests

uninstall:
	@rm -f $(PREFIX)/include/ss.h
	@rm -f $(PREFIX)/lib/libss.a
	@echo "Removed ss_string.h header from $(PREFIX)/include/ss.h"
	@echo "Removed static library libss.a from $(PREFIX)/lib/libss.a"

clear:
	@rm -rf lib
	@rm -rf tests/tests