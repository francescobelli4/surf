TARGET = surf

CC = gcc

CFLAGS = -Wall

# Compila e crea l'eseguibile
all: $(TARGET)

$(TARGET):  
	$(CC) $(CFLAGS) -o $(TARGET) surf.c options.c terminal_table.c

clean:
	rm -f $(TARGET)