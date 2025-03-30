TARGET = surf

CC = gcc

CFLAGS = -Wall

# Compila e crea l'eseguibile
all: $(TARGET)

$(TARGET):  
	$(CC) $(CFLAGS) -o bin/$(TARGET) src/surf.c src/options.c src/terminal_table.c src/utils.c

clean:
	rm -f bin/$(TARGET)