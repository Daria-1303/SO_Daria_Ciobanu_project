# folders for executables
BIN_DIR = bin
BIN_TREASURE_MANAGER = treasure_manager

# compiler + flags
CC = gcc
CFLAGS = -Wall

# MAIN SOURCE
SRC_MANAGER = src/treasure_manager/treasure_manager.c

# libraries
SRC_treasure_manager = $(shell find src/treasure_manager/Libraries -name "*.c")
init:
	@echo "Initializing project..."

build:
	@echo "Building project..."
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(BIN_TREASURE_MANAGER) $(SRC_treasure_manager) $(SRC_MANAGER)
	@echo "Build complete."

run:
	@echo "Running project..."
	./$(BIN_DIR)/$(BIN_TREASURE_MANAGER) 
	@echo "Run complete."

clean:
	@echo "Cleaning project..."
	rm -f $(BIN_DIR)/$(BIN_TREASURE_MANAGER)
	@echo "Clean complete."