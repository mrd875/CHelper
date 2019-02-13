# Targets to make by default
TARGETS = $(LIBCHELPER)

# Flags
CC = gcc
CFLAGS = -g #-D __CHELPDEBUG__
CPPFLAGS = -std=gnu99 -Wall -Wextra -pedantic


# Setup dirs
BUILD_DIR = build
SRC_DIR = src
BIN_DIR = $(BUILD_DIR)/bin
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj


# setup target names
LIBCHELPER = $(LIB_DIR)/libCHelper.a

TESTLIST = $(BIN_DIR)/testlist
TESTDICT = $(BIN_DIR)/testdict
TESTSTRING = $(BIN_DIR)/teststring
TESTSTRINGBUILDER = $(BIN_DIR)/teststringbuilder

TESTS = $(TESTLIST) $(TESTDICT) $(TESTSTRING) $(TESTSTRINGBUILDER)


# default rule and all
all: mkdirs $(TARGETS)


tests: mkdirs $(TESTS)


# some target rules
libchelper: mkdirs $(LIBCHELPER)

testlist: mkdirs $(TESTLIST)
	@echo "Running test list"
	@echo ""
	@$(TESTLIST)

testdict: mkdirs $(TESTDICT)
	@echo "Running test list"
	@echo ""
	@$(TESTDICT)

teststring: mkdirs $(TESTSTRING)
	@echo "Running test list"
	@echo ""
	@$(TESTSTRING)

teststringbuilder: mkdirs $(TESTSTRINGBUILDER)
	@echo "Running test list"
	@echo ""
	@$(TESTSTRINGBUILDER)


# mkdirs and clean rules
mkdirs:
	@echo "making dirs"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LIB_DIR)
	@echo ""

clean:
	@echo "cleaning intermediate files"
	@rm -rf $(OBJ_DIR)

clean-binary:
	@echo "cleaning binaries"
	@rm -rf $(TARGETS)
	@rm -rf $(BIN_DIR) $(LIB_DIR)

clean-all: clean-binary clean
	@echo "cleaning all"
	@rm -rf $(BUILD_DIR)


# src dirs
CHELPER_DIR = $(SRC_DIR)/chelper
CHELPER_INTERNAL_DIR = $(CHELPER_DIR)/internal

TEST_DIR = $(SRC_DIR)/test


# src lists
CHELPER_SOURCES = $(wildcard $(CHELPER_DIR)/*.c)
CHELPER_INTERNAL_SOURCES = $(wildcard $(CHELPER_INTERNAL_DIR)/*.c)

TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)


# obj lists
CHELPER_OBJS = $(patsubst $(CHELPER_DIR)/%.c, $(OBJ_DIR)/chelper_%.o, $(CHELPER_SOURCES))
CHELPER_INTERNAL_OBJS = $(patsubst $(CHELPER_INTERNAL_DIR)/%.c, $(OBJ_DIR)/chelper_internal_%.o, $(CHELPER_INTERNAL_SOURCES))


TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/test_%.o, $(TEST_SOURCES))



# rules to build objects
$(OBJ_DIR)/chelper_%.o: $(CHELPER_DIR)/%.c
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

$(OBJ_DIR)/chelper_internal_%.o: $(CHELPER_INTERNAL_DIR)/%.c
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<


# linking rules
$(LIBCHELPER): $(CHELPER_OBJS) $(CHELPER_INTERNAL_OBJS)
	@echo "ar $@"
	@ar rcs $@ $^
	@echo ""

$(TESTDICT): $(OBJ_DIR)/test_dictionary.o $(LIBCHELPER)
	@echo "$(CC) $@"
	@$(CC) -o $@ $(CFLAGS) $^
	@echo "finished $@"
	@echo ""

$(TESTLIST): $(OBJ_DIR)/test_list.o $(LIBCHELPER)
	@echo "$(CC) $@"
	@$(CC) -o $@ $(CFLAGS) $^
	@echo "finished $@"
	@echo ""

$(TESTSTRING): $(OBJ_DIR)/test_string.o $(LIBCHELPER)
	@echo "$(CC) $@"
	@$(CC) -o $@ $(CFLAGS) $^
	@echo "finished $@"
	@echo ""

$(TESTSTRINGBUILDER): $(OBJ_DIR)/test_stringbuilder.o $(LIBCHELPER)
	@echo "$(CC) $@"
	@$(CC) -o $@ $(CFLAGS) $^
	@echo "finished $@"
	@echo ""
