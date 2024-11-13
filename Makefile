# Define the compiler
CC = gcc

# Define the source file and output directory
SRC    = src/main.c
TARGET = build/web_server

# Define the paths for include and library files
MONGO_INCLUDE      = /opt/homebrew/Cellar/mongo-c-driver/1.29.0/include
MONGO_LIB          = /opt/homebrew/Cellar/mongo-c-driver/1.29.0/lib
MICROHTTPD_INCLUDE = /opt/homebrew/include
MICROHTTPD_LIB     = /opt/homebrew/lib

# Compilation flags for includes and libraries
CFLAGS  = -I$(MONGO_INCLUDE)/libmongoc-1.0 -I$(MONGO_INCLUDE)/libbson-1.0 -I$(MICROHTTPD_INCLUDE)
LDFLAGS = -L$(MONGO_LIB) -L$(MICROHTTPD_LIB) -lmongoc-1.0 -lbson-1.0 -lmicrohttpd

# Build target
$(TARGET): $(SRC)
	mkdir -p $(dir $(TARGET)) # Create build directory if it doesn't exist
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: $(TARGET)
	./build/web_server

# Clean target
.PHONY: clean
clean:
	rm -f $(TARGET)

