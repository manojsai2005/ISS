# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Target names
TARGETS = naming_server storage_server

# Object files
OBJ_NAMING = naming_server.o helper.o
OBJ_STORAGE = storage_server.o helper.o

# Port and IP configuration
IPADD = 127.0.0.1
PORT = 9001

# Default target
all: $(TARGETS)

# Naming Server executable
naming_server: $(OBJ_NAMING)
	$(CC) $(CFLAGS) -o naming_server $(OBJ_NAMING)

# Storage Server executable
storage_server: $(OBJ_STORAGE)
	$(CC) $(CFLAGS) -o storage_server $(OBJ_STORAGE)

# Compile naming_server.c
naming_server.o: naming_server.c helper.h
	$(CC) $(CFLAGS) -c naming_server.c

# Compile storage_server.c
storage_server.o: storage_server.c helper.h
	$(CC) $(CFLAGS) -c storage_server.c

# Compile helper.c
helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c

# Clean up
clean:
	rm -f $(TARGETS) *.o

# Run Naming Server
ns: naming_server
	./naming_server $(PORT)

# Run Storage Server
ss: storage_server
	./storage_server $(IPADD) $(PORT)
