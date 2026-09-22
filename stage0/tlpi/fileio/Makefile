TLPI_DIR = ../tlpi-dist
TLPI_LIB = ${TLPI_DIR}/libtlpi.a
TLPI_INCL_DIR = $(TLPI_DIR)/lib

CC      = gcc

CFLAGS = -std=c99 -D_XOPEN_SOURCE=600 \
		-D_DEFAULT_SOURCE \
		-g -I${TLPI_INCL_DIR} \
		-pedantic \
		-Wall \
		-W \
		-Wmissing-prototypes \
		-Wimplicit-fallthrough \
		-Wno-unused-parameter \

TARGETS   = copy seek_io tee cp bad_exclusive_open t_readv large_file large_file2 append atomic_append dup verify_dup

.PHONY: all
all:
	@for target in $(TARGETS); do \
		echo "Compiling $$target..."; \
		$(CC) $(CFLAGS) $$target.c -o $$target $(TLPI_LIB); \
	done

.PHONY: clean
clean:
	rm -f $(TARGETS)
