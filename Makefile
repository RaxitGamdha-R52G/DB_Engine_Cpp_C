CC := gcc

HELLO_SRC := examples/hello/main.c
HELLO_BIN := build/examples/hello.exe

hello: $(HELLO_BIN)

$(HELLO_BIN): $(HELLO_SRC)
	@mkdir -p build/examples
	$(CC) $< -o $@

run-hello: hello
	./$(HELLO_BIN)

# Example targets
.PHONY: hello run-hello