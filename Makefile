CC = gcc
CFLAGS = -Wall -I/usr/include/libxml2/
LFLAGS 	= -Ilib `xml2-config --cflags` `xml2-config --libs` `sdl2-config --cflags` `sdl2-config --libs`
EXEC=carte
SRC_PATH = src/
HEADER_PATH = header/
all:  $(EXEC)

carte: main.o affichage.o AnalyseDuFichier.o
	$(CC) $(CFLAGS) -o $@  $^ -lxml2 `sdl2-config --cflags --libs` -lSDL2_gfx

affichage.o: $(SRC_PATH)affichage.c $(HEADER_PATH)AnalyseDuFichier.h $(HEADER_PATH)affichage.h
	$(CC) -o $@ -c $< $(LFLAGS)
	
AnalyseDuFichier.o: $(SRC_PATH)AnalyseDuFichier1.c $(HEADER_PATH)AnalyseDuFichier.h
	$(CC) -o $@ -c $< $(LFLAGS)
	
main.o: $(SRC_PATH)main.c $(HEADER_PATH)AnalyseDuFichier.h
	$(CC) -o $@ -c $< $(LFLAGS)
clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)
