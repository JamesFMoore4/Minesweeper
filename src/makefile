CC = gcc
CLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
CFLAGS = -Wall -Wextra
BIN = mines
OBJECTS = main.o tile.o
INCLUDE = -I ../include/
LIB = -L ../lib/

all: $(BIN)

$(BIN): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN) $(CLIBS) $(INCLUDE) $(LIB)

%.o: %.c
	@$(CC) -c $^ -o $@
clean:
	@rm $(OBJECTS) $(BIN)

