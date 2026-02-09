SOURCE=$(wildcard src/*.c)
INCLUDE=$(wildcard include/*.h)
CFLAGS=-Wall -Wextra -Wno-unused-parameter
TARGET=bfc
BFSOURCE=brainfuck.bf
BFTARGET=output

$(TARGET): $(SOURCE) $(INCLUDE)
	@gcc $(CFLAGS) $(SOURCE) -o $(TARGET)

$(BFTARGET): $(TARGET) $(BFSOURCE)
	@./$(TARGET) $(BFSOURCE) $(BFTARGET)

run: $(BFTARGET)
	@bash -c './$(BFTARGET); ret=$$?; echo; echo "Return code: $$ret"'


dump: $(BFTARGET)
	@echo "--- Hex dump ---" 
	@hexdump -C $(BFTARGET)

read: $(BFTARGET)
	@echo "--- ELF Dump ---" 
	@readelf -a $(BFTARGET)

test: dump read
	@echo "--- Program Output ---" 
	@bash -c './$(BFTARGET); ret=$$?; echo; echo "Return code: $$ret"'