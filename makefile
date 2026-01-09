SOURCE=$(wildcard src/*.c)
INCLUDE=$(wildcard include/*.h)
CFLAGS=-Wall -Wextra
TARGET="bfc"

$(TARGET): $(SOURCE) $(INCLUDE)
	@gcc $(CFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	@./$(TARGET)