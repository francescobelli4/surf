TARGET = surf

CC = gcc

CFLAGS = -Wall

# Compila e crea l'eseguibile
all: $(TARGET)

$(TARGET):  
	$(CC) $(CFLAGS) -o $(TARGET) src/surf.c src/options.c src/terminal_table.c

clean:
	rm -f $(TARGET)