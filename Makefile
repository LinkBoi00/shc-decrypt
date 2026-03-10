CC = gcc
OUT_DIR = out

CFLAGS_HOOK = -shared -fPIC
LDFLAGS_HOOK = -ldl

all: hooks main

main:
	mkdir -p $(OUT_DIR)
	$(CC) main.c -o $(OUT_DIR)/shc-decrypt

hooks:
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS_HOOK) hooks.c -o $(OUT_DIR)/hooks.so $(LDFLAGS)

clean:
	rm -rf $(OUT_DIR)

.PHONY: all hooks clean main
