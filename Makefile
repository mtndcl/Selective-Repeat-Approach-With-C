CC = gcc

TARGET = m

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
