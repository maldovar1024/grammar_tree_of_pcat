PWD = $(shell pwd)
INCLUDE_PATH = include

CC = g++
CPPFLAGS = -std=c++17 -I $(INCLUDE_PATH) -Wall

SRC_DIR = src
OBJ_DIR = obj
DEP_DIR = dep
PCAT_DIR = tests

ENTRY = main.cc

vpath %.cc $(SRC_DIR)

GRAM_SRC = parser_src/parser.yy
GRAM_CC = $(SRC_DIR)/parser.cc
LEX_SRC = parser_src/scanner.l
LEX_CC = $(SRC_DIR)/scanner.cc

SRC = $(wildcard $(SRC_DIR)/*.cc) $(GRAM_CC) $(LEX_CC) $(ENTRY)
OBJ = $(addprefix $(OBJ_DIR)/, $(patsubst %.cc, %.o, $(notdir $(SRC))))
DEP = $(addprefix $(DEP_DIR)/, $(patsubst %.cc, %.d, $(notdir $(SRC))))

TARGET = _pcat_parser
DEBUG_OUT = debug.txt

.PHONY: build run debug clean-all clean clean-parser

build: $(TARGET)

run: $(TARGET)
	./$< $(PCAT_DIR)/$(FILE).pcat

debug: $(TARGET)
	./$< $(PCAT_DIR)/$(FILE).pcat -d

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: %.cc | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) -o $@ -c $<

$(LEX_CC): $(LEX_SRC)
	flex -o $@ $<

LOCATION_HH = include/location.hh
GRAM_HH = include/parser.hh

$(LOCATION_HH) $(GRAM_HH): $(GRAM_CC)

$(GRAM_CC): $(GRAM_SRC)
	bison -Dapi.location.file='"$(PWD)/$(LOCATION_HH)"' -o $@ $<

$(DEP_DIR)/%.d: %.cc | $(DEP_DIR) $(GRAM_HH) $(LOCATION_HH)
	@set -e; rm -f $@; \
	$(CC) $(CPPFLAGS) -MM $< > $@.$$$$; \
	sed 's,\(.*\)\.o[ :]*,$(OBJ_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(DEP)

$(OBJ_DIR) $(DEP_DIR):
	mkdir $@

clean-all: clean clean-parser

clean: 
	-rm $(TARGET) $(OBJ_DIR)/*.o $(DEP_DIR)/*.d
	
clean-parser:
	-rm $(LEX_CC) $(GRAM_CC) $(INCLUDE_PATH)/parser.hh $(LOCATION_HH)
