CXX ?= g++

### Path
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = bin

### Executable
BIN_NAME = main

### Extensions
SRC_EXT = cc

### Code Lists
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' -printf '%T@ %p\n' | sort -k 1nr | cut -d ' ' -f 2)
#SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)


#TODO find better way to include all header files
GROUNDSTATION_H_DIR = $(SRC_PATH)/groundstation/header/
TERMINAL_H_DIR = $(SRC_PATH)/terminal/header/
CUBESAT_H_DIR = $(SRC_PATH)/cubesat/header/
TCP_H_DIR = $(SRC_PATH)/tcp/header/
BATTERIES_H_DIR = $(SRC_PATH)/emulation/batteries/header/
POWER_H_DIR = $(SRC_PATH)/emulation/power/header/
SUBSYSTEM_H_DIR = $(SRC_PATH)/emulation/subsystem/header/
ACDS_H_DIR = $(SRC_PATH)/emulation/AdcsEmulator/header/
ATTITUDE_H_DIR = $(SRC_PATH)/emulation/AttitudeSensor/header/
SUB_STRUCTS_H_DIR = $(SRC_PATH)/emulation/SubsystemStateStructs/header/
HW_EMULATION_SERVER_H_DIR = $(SRC_PATH)/emulation/HardwareEmulationServer/header/
COMMS_EMULATOR_H_DIR = $(SRC_PATH)/emulation/CommsEmulator/header/

INCLUDES = -I $(GROUNDSTATION_H_DIR) -I $(TERMINAL_H_DIR) -I $(CUBESAT_H_DIR) -I $(BATTERIES_H_DIR) -I $(POWER_H_DIR) -I $(TCP_H_DIR) -I $(SUBSYSTEM_H_DIR) -I $(ACDS_H_DIR) -I $(ATTITUDE_H_DIR) -I $(SUB_STRUCTS_H_DIR) -I $(HW_EMULATION_SERVER_H_DIR) -I $(COMMS_EMULATOR_H_DIR)

### Build flags for all targets
CFLAGS    = -std=c++14
CXXFLAGS    = -std=c++14

# Space separated pkg-config libraries used by this project
LIBS = 

### Build tools
#CC          = g++
#COMP        = $(CC) $(CFLAGS) -o $@ -c $<
#LINK        = $(CC) $(CFLAGS) -o $@ $^ 
#COMPLINK    = $(CC) $(CFLAGS) -o $@ $< 


.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean: 
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)


.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable 
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@


### Standard parts
#program		:= output
#extra_clean	:=

### Include files
#all:

#include src/terminal/header/module.mk

### General rules
#%.o:	%.cpp
#	$(COMP)

### Rules
#.PHONY: all
#all:	$(program)

#.PHONY: clean
#clean:
#	rm $(program) $(extra_clean)

### Desired program output
#output: obj/terminal_test.o
#	$(LINK)
