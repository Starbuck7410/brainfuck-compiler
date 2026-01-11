SOURCE=$(wildcard src/*.c)
INCLUDE=$(wildcard include/*.h)
CFLAGS=-Wall -Wextra
TARGET="bfc"

$(TARGET): $(SOURCE) $(INCLUDE)
	@gcc $(CFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	@./$(TARGET)

dump: run
	@echo "--- Hex dump ---" 
	@hexdump -C output

read: run
	@echo "--- ELF Dump ---" 
	@readelf -a output

test: dump read
	@echo "--- Program Output ---" 
	@bash -c './output; ret=$$?; echo "Return code: $$ret"'