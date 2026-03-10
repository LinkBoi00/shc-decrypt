CC = gcc
OUT_DIR = out

CFLAGS_HOOK = -shared -fPIC
LDFLAGS_HOOK = -ldl

all: hooks

hooks:
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS_HOOK) hooks.c -o $(OUT_DIR)/hooks.so $(LDFLAGS)

clean:
	rm -rf $(OUT_DIR)

.PHONY: all hooks clean
