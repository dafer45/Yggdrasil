# Copyright 2016 Anna Sinelnikova and Kristofer Björnson
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

## @file Makefile
## @brief ...
##
## Run $make list_of_objects to see
## the rule for compilation only onle file.
##
## @author Anna Sinelnikova
## @author Kristofer Björnson
##
## @cond

# Main compiler
CC:= g++

# Compiler flags
CFLAGS:= -std=c++11 -Wall -O3

# Libraries (can also include path to these libraries as -L/SOME_PATH)
LIB:= -lm

# Optimization (-O3 or -O2)
OPT:= -O3

# Directory for executables
BIN_DIR:= .

# Name for exectutable
STATIC_LIB = build/libYggdrasil.a

# Root directories for .h files
INC_DIR = include/Streams/
INC_DIR += include/Buffers/

# Main source directory
SRC_DIR = src/Streams/
SRC_DIR += src/Buffers/

# Directory for Objects
OBJ_DIR:= build


#---------------------No need to change the following!-------------------

# Include files are needed fot Includes
INC_FILES :=  $(wildcard $(INC_DIR)/*.h))

# All include directories are needed for Includes
INC_DIRS:= $(dir $(INC_FILES))

# Includes: -I ...
LDLIBS:=$(addprefix -I, $(INC_DIRS))

# Source files
SRC :=  $(foreach srcDir, $(SRC_DIR), $(wildcard $(srcDir)/*.cpp))

# Object files
OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

#All object files
ALL_OBJ := $(OBJ)

# File names of objects without directory name
OBJ_PURE := $(notdir $(OBJ))

all: $(STATIC_LIB)

# Linking
$(STATIC_LIB): $(OBJ) $(CUDA_OBJ)
	@echo "Linking: " $(notdir $(STATIC_LIB))
	@ar rcs $(STATIC_LIB) $(OBJ) $(CUDA_OBJ)

# Compilation rule
define app_compile_template
 $(1)_OBJ = $$(addprefix $$(OBJ_DIR)/, $$(notdir $$(patsubst %.cpp, %.o, $(1))))

$$($(1)_OBJ): $(1)
	@echo "Compiling: $(1)"
	@$$(CC) $$(CFLAGS) $$(OPT) -c $(1) -o $$($(1)_OBJ) $$(LDLIBS)
endef

# Compile
$(foreach app, $(SRC), $(eval $(call app_compile_template,$(app))))

# Cleaning
clean:
	@echo "Cleaning: build/"
	@rm -r build/*

list_of_objects:
	@echo $(OBJ)

# For debugging makefile
print:
	@echo $(SRC_DIR)
	@echo $(SRC)
	@echo $(LDLIBS)

## @endconf

