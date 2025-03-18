##############################################################################
# huffman.cpp: depende de huffman.h 
##############################################################################

MAIN := huffman

OBJECTS := main.o huffman.o bwt.o mtf.o
DEBUG   := -g
FLAGS   := -Wall -Wextra -std=c++17 -pedantic-errors $(DEBUG)
CC      := g++

ifeq ($(OS),Windows_NT)
	OUTPUTMAIN := $(MAIN).exe
else
	OUTPUTMAIN := $(MAIN).out
endif

.PHONY: all clean run

all: $(OUTPUTMAIN)
	@echo "Compilação completa!"

$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN)

main.o: main.cpp huffman.h
	$(CC) $(FLAGS) -c main.cpp

huffman.o: huffman.cpp huffman.h
	$(CC) $(FLAGS) -c huffman.cpp

bwt.o: bwt.cpp bwt.h
	$(CC) $(FLAGS) -c bwt.cpp

mtf.o: mtf.cpp mtf.h
	$(CC) $(FLAGS) -c mtf.cpp

clean:
	rm -rf $(OBJECTS) $(OUTPUTMAIN)
	@echo "Cleanup completo!"

run: all
	./$(OUTPUTMAIN) C exemplo1.txt output1.huff
	@echo "Compressão completa!"
	./$(OUTPUTMAIN) D output1.huff exemplo1Huff.txt
	@echo "Decompressão completa!"
