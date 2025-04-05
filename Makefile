TARGET = surf

CC = gcc

CFLAGS = -Wall

# Compila e crea l'eseguibile
compile: 
	$(CC) $(CFLAGS) -o bin/$(TARGET) src/surf.c src/options.c src/terminal_table.c src/terminal_short.c src/utils.c src/files.c -lm
	mkdir -p locale/it_IT/LC_MESSAGES
	mkdir -p locale/en_US/LC_MESSAGES
	msgfmt -o locale/en_US/LC_MESSAGES/surf.mo locale/surf_en.pot
	msgfmt -o locale/it_IT/LC_MESSAGES/surf.mo locale/surf_it.pot

install:
	$(CC) $(CFLAGS) -o bin/$(TARGET) src/surf.c src/options.c src/terminal_table.c src/terminal_short.c src/utils.c src/files.c -lm
	cp bin/$(TARGET) /usr/local/bin/$(TARGET)

	mkdir -p /usr/share/locale/en_US/LC_MESSAGES/
	mkdir -p /usr/share/locale/it_IT/LC_MESSAGES/

	cp locale/it_IT/LC_MESSAGES/surf.mo /usr/share/locale/it_IT/LC_MESSAGES/
	cp locale/en_US/LC_MESSAGES/surf.mo /usr/share/locale/en_US/LC_MESSAGES/

clean:
	rm -f /usr/local/bin/$(TARGET)
	rm -f bin/$(TARGET)
	rm -f /usr/share/locale/it_IT/LC_MESSAGES/surf.mo
	rm -f /usr/share/locale/en_US/LC_MESSAGES/surf.mo