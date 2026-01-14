SOURCE=$(wildcard src/*.c)
INCLUDE=$(wildcard include/*.h)
CFLAGS=-Wall -Wextra
TARGET=bfc
BFSOURCE=brainfuck.bf
BFTARGET=output

$(TARGET): $(SOURCE) $(INCLUDE)
	@gcc $(CFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	@./$(TARGET) $(BFSOURCE) $(BFTARGET)

dump: run
	@echo "--- Hex dump ---" 
	@hexdump -C $(BFTARGET)

read: run
	@echo "--- ELF Dump ---" 
	@readelf -a $(BFTARGET)

test: dump read
	@echo "--- Program Output ---" 
	@bash -c './$(BFTARGET); ret=$$?; echo; echo "Return code: $$ret"'