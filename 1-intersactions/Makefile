PROJECT = figures
BUILD   = g++
MAKEDIR = mkdir
REMOVE  = rm -rf
OUT_DIR = bin
OUTPUT  = ./$(OUT_DIR)/$(PROJECT)

DBGFLAGS = -g
CFLAGS  = -flto
#-I.\include
LDFLAGS = -flto
#-L. -l:pdcurses.a -s

EXCLUDE = catch.cpp
TARGETS = main.cpp tests.cpp
ALLSRCS = $(wildcard *.cpp)
SOURCES = $(filter-out $(TARGETS) $(EXCLUDE),$(ALLSRCS))
OBJECTS = $(SOURCES:.cpp=.o)

.SECONDEXPANSION:
.PHONY: all cov test debug build list clean

all: build clean

list:
	@echo -e "Targets: $(TARGETS)\nSources: $(SOURCES)\nObjects: $(OBJECTS)"

cov: CFLAGS = -fprofile-arcs -ftest-coverage
cov: LDFLAGS += -fprofile-arcs -ftest-coverage
cov: test
	@$(OUTPUT)
	@gcov geometry.cpp | head

# test: CFLAGS += $(DBGFLAGS)
test: SOURCES += catch.cpp
test: $(OUT_DIR) catch.o tests

debug: CFLAGS += $(DBGFLAGS)
debug: build

build: $(OUT_DIR) main


$(OUT_DIR):
	$(MAKEDIR) $(OUT_DIR)

$(TARGETS:.cpp=): $(OBJECTS) $$(@).o
	$(BUILD) -o $(OUTPUT) $(OBJECTS) $(@).o $(LDFLAGS)
# $(BUILD) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(BUILD) -o $@ -c $< $(CFLAGS)

clean:
	$(REMOVE) *.o *.gc*
