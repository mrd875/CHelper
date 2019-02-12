# Mason Demerais
# mrd875
# 11202338


CC = gcc
CFLAGS = -g
CPPFLAGS = -std=gnu99 -Wall -Wextra -pedantic

TARGETS = 


PLATFORM = $(shell uname -s)
ARCH = $(shell uname -m)

BUILD = ./build

OBJ = $(BUILD)/obj
BIN = $(BUILD)/bin
LIB = $(BUILD)/lib
SRC = .

TESTLIST = $(BUILD)/testlist


.phony:	all

all: mkdirs $(TARGETS)

mkdirs:
	@echo "making dirs"
	@mkdir -p $(BUILD)
	@mkdir -p $(OBJ)
	@mkdir -p $(BIN)
	@mkdir -p $(LIB)
	@mkdir -p $(OBJ)/test
	@mkdir -p $(OBJ)/internal
	@echo ""

clean:
	@echo "cleaning intermediate files"
	@rm -rf $(OBJ)

clean-binary:
	@echo "cleaning binaries"
	@rm -rf $(TARGETS)
	@rm -rf $(BIN) $(LIB)

clean-all: clean-binary clean
	@echo "cleaning all"
	@rm -rf $(BUILD)

listtest: mkdirs $(TESTLIST)



$(TESTLIST): $(OBJ)/test/list.o $(OBJ)/common.o $(OBJ)/list.o $(OBJ)/internal/common.o $(OBJ)/string.o
	@echo "$(CC) $@"
	@$(CC) -o $@ $(CFLAGS) $^
	@echo "finished $@"
	@echo ""

$(OBJ)/test/list.o: $(SRC)/test/list.c $(SRC)/common.h $(SRC)/list.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<




$(OBJ)/internal/common.o: $(SRC)/internal/common.c $(SRC)/internal/common.h $(SRC)/string.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<




$(OBJ)/common.o: $(SRC)/common.c $(SRC)/common.h $(SRC)/internal/common.h $(SRC)/string.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

$(OBJ)/string.o: $(SRC)/string.c $(SRC)/string.h $(SRC)/common.h $(SRC)/internal/common.h $(SRC)/list.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<
	
$(OBJ)/list.o: $(SRC)/list.c $(SRC)/list.h $(SRC)/common.h $(SRC)/internal/common.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<
