

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

static-lib:
	@mkdir -p lib
	@rm -f lib/*
	@gcc -c src/ss_alloc.c src/ss_string.c src/ss_string_iter.c
	@mv *.o lib/
	@ar rcs lib/libss.a lib/ss_alloc.o lib/ss_string.o lib/ss_string_iter.o
	@rm -f lib/*.o

install: static-lib install-static-lib install-headers

install-static-lib:
	@echo "Copying static library libss.a to $(PREFIX)/lib/libss.a"
	@cp lib/libss.a $(PREFIX)/lib/
	@echo "Done."

install-headers:
	@echo "Copying ss_string.h header to $(PREFIX)/include/ss.h"
	@cp src/ss_string.h $(PREFIX)/include/ss.h
	@echo Header file installed.

uninstall:
	@rm -f $(PREFIX)/include/ss.h
	@rm -f $(PREFIX)/lib/libss.a