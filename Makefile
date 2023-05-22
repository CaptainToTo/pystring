CC     = clang
CFLAGS = -Wall -Wextra -Werror -pedantic -Wstrict-prototypes -gdwarf-4

EXE = str-handle
OBJ = handle.o

all: clean $(EXE)
	valgrind ./$(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

.PHONY: clean

clean:
	rm -f *.o $(EXE)