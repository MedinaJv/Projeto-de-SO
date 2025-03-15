
##############################################################################
# huffman.cpp: depende de huffman.h 
##############################################################################

MAIN := huffman

OBJECTS := main.o huffman.o
FLAGS := -Wall -Wextra -std=c++17 -pedantic-errors
DEBUG := -g
CC := g++

ifeq ($(OS), Windows_NT)
OUTPUTMAIN := $(MAIN).exe
else
OUTPUTMAIN := $(MAIN).out
endif

all: $(OUTPUTMAIN)
	@echo Compiling 'all' completo!

$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN)

main.o: main.cpp huffman.h
	$(CC) $(FLAGS) -c main.cpp

huffman.o: huffman.cpp huffman.h
	$(CC) $(FLAGS) -c huffman.cpp

clean:
	rm -rf $(OBJECTS) $(OUTPUTMAIN)
	@echo Cleanup completo!!!

run: all
	./$(OUTPUTMAIN) C input.txt output.huff
	@echo Compressão completa!!!
	./$(OUTPUTMAIN) D output.huff output.txt
	@echo Decompressão completa!!!