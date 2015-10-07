##
#  Simple makefile to ease the compilation/link process
##

TARGET := sudoku-solver

# FIX THIS
# Retrieve makefile directory.
# Call this make file from any directory does not affect the compilation ;)
#MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
#CURRENT_DIR := $(notdir $(patsubst %/,%,$(dir $MKFILE_PATH)))

# Directories
ROOT := $(shell pwd)
SRCDIR := $(ROOT)/src
INCDIR := $(ROOT)/include
LIBDIR := $(ROOT)/lib
BUILDDIR := $(ROOT)/build

RELDIR := $(BUILDDIR)/release
DEBDIR := $(BUILDDIR)/debug

ROBJDIR := $(RELDIR)/obj
RBINDIR := $(RELDIR)/bin
DOBJDIR := $(DEBDIR)/obj
DBINDIR := $(DEBDIR)/bin

RBINARY := $(RBINDIR)/$(TARGET)
DBINARY := $(DBINDIR)/$(TARGET)

# Project files
CCSRCS := $(shell find $(ROOT) -name "*.cpp")
CCHDRS := $(shell find $(ROOT) -name "*.hpp")

CCOBJS = $(CCSRCS:.cpp=.o)
ROBJS := $(addprefix $(ROBJDIR)/, $(CCOBJS))
DOBJS := $(addprefix $(DOBJDIR)/, $(CCOBJS))

# Flags
INC_PATHS := -I$(INCDIR)
LIB_PATHS := -L$(LIBDIR) -L$(LIBDIR)/picosat

LIBS := -lpicosat

CXXFLAGS = -Wall -Wextra $(INC_PATHS)
LDFLAGS = -Wall $(LIB_PATHS) $(LIBS)

## Special rules
.PHONY: all clean mkdir-release mkdir-debug

## all
all: debug

## release/debug rules
release: mkdir-release $(RBINARY)
debug: mkdir-debug $(DBINARY)

# Binaries dependencies
$(RBINARY): $(ROBJS)
$(DBINARY): $(DOBJS)

## Compile options
$(ROBJDIR)/%.o: CXXFLAGS += -Os -DNDEBUG
$(DOBJDIR)/%.o: CXXFLAGS += -O0 -g

## Link options
#$(RBINARY): LDFLAGS +=
#$(RBINARY): LDFLAGS +=

## Build rules
$(ROBJDIR)/%.o $(DOBJDIR)/%.o: %.cpp $(CCHDRS)
	@echo "Compiling: $< -> $@"
	@echo "    Flags: $(CXXFLAGS)"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(RBINARY) $(DBINARY):
	@echo "Linking: $@"
	@echo "  Flags: $(LDFLAGS)"
	@$(CXX) $^ $(LDFLAGS) -o $@

# Help rules
mkdir-release:
	@mkdir -p $(ROBJDIR) $(RBINDIR)

mkdir-debug:
	@mkdir -p $(DOBJDIR) $(DBINDIR)

clean:
	@echo "Cleaning object files"
	@$(RM) -v $(ROBJS) $(DOBJS)
	@echo "Cleaning binaries"
	@$(RM) -v $(RBINARY) $(DBINARY)
