# folders for executables
BIN_DIR = bin
BIN_TREASURE_MANAGER = treasure_manager
BIN_TREASURE_MONITOR = treasure_monitor

# compiler + flags
CC = gcc
CFLAGS = -Wall

# MAIN SOURCE
SRC_MANAGER = src/treasure_manager/treasure_manager.c
SRC_MONITOR_MAIN = src/treasure_hub/Monitor/main.c
SRC_MONITOR_LIB = src/treasure_hub/Monitor/monitor.c

# libraries
SRC_treasure_manager = $(shell find src/treasure_manager/Libraries -name "*.c")
SRC_treasure_monitor = $(shell find src/treasure_hub/Monitor -name "*.c")
init:
	@echo "Initializing project..."

build:
	@echo "Building project..."
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(BIN_TREASURE_MANAGER) $(SRC_treasure_manager) $(SRC_MANAGER)
	@echo "Build complete."

	@echo "Building treasure_monitor..."
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(BIN_TREASURE_MONITOR) $(SRC_MONITOR_MAIN) $(SRC_MONITOR_LIB)
	@echo "Build complete."
clean:
	@echo "Cleaning project..."
	rm -f $(BIN_DIR)/$(BIN_TREASURE_MANAGER)
	@echo "Clean complete."