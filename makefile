CC = gcc
TARGET = 46mcshell

compile: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c

run: 
	./${TARGET}

clean:
	$(RM) $(TARGET)