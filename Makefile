CC = gcc
CFLAGS = -g #-D __CHELPDEBUG__
CPPFLAGS = -std=gnu99 -Wall -Wextra -pedantic

TARGETS = $(LIBCHELPER)


PLATFORM = $(shell uname -s)
ARCH = $(shell uname -m)

BUILD = ./build/$(PLATFORM)$(ARCH)

OBJ = $(BUILD)/obj
BIN = $(BUILD)/bin
LIB = $(BUILD)/lib
SRC = .

LIBCHELPER = $(LIB)/libCHelper.a
TESTLIST = $(BIN)/testlist
TESTDICT = $(BIN)/testdict


.phony:	all

all: mkdirs $(TARGETS)

mkdirs:
	@echo "making dirs"
	@mkdir -p $(BUILD)
	@mkdir -p $(OBJ)
	@mkdir -p $(BIN)
	@mkdir -p $(LIB)
	@mkdir -p $(OBJ)/test
	@mkdir -p $(OBJ)/lib
	@mkdir -p $(OBJ)/lib/internal
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

testlist: mkdirs $(TESTLIST)
testdict: mkdirs $(TESTDICT)
libchelper: mkdirs $(LIBCHELPER)



$(TESTDICT): $(OBJ)/test/dictionary.o $(LIBCHELPER)
	@echo "$(CC) $@"
	@$(CC) -o $@ $(CFLAGS) $^
	@echo "finished $@"
	@echo ""

$(OBJ)/test/dictionary.o: $(SRC)/test/dictionary.c $(SRC)/lib/common.h $(SRC)/lib/string.h $(SRC)/lib/dictionary.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<



$(TESTLIST): $(OBJ)/test/list.o $(LIBCHELPER)
	@echo "$(CC) $@"
	@$(CC) -o $@ $(CFLAGS) $^
	@echo "finished $@"
	@echo ""

$(OBJ)/test/list.o: $(SRC)/test/list.c $(SRC)/lib/common.h $(SRC)/lib/list.h $(SRC)/lib/string.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<




$(LIBCHELPER): $(OBJ)/lib/internal/common.o $(OBJ)/lib/common.o $(OBJ)/lib/string.o $(OBJ)/lib/list.o $(OBJ)/lib/dictionary.o
	@echo "ar $@"
	@ar rcs $@ $^
	@echo ""


$(OBJ)/lib/internal/common.o: $(SRC)/lib/internal/common.c $(SRC)/lib/internal/common.h $(SRC)/lib/string.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

$(OBJ)/lib/common.o: $(SRC)/lib/common.c $(SRC)/lib/common.h $(SRC)/lib/internal/common.h $(SRC)/lib/string.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

$(OBJ)/lib/string.o: $(SRC)/lib/string.c $(SRC)/lib/string.h $(SRC)/lib/common.h $(SRC)/lib/internal/common.h $(SRC)/lib/list.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<
	
$(OBJ)/lib/list.o: $(SRC)/lib/list.c $(SRC)/lib/list.h $(SRC)/lib/common.h $(SRC)/lib/internal/common.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

$(OBJ)/lib/dictionary.o: $(SRC)/lib/dictionary.c $(SRC)/lib/dictionary.h $(SRC)/lib/internal/common.h $(SRC)/lib/common.h $(SRC)/lib/list.h $(SRC)/lib/string.h
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<
