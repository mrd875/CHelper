# Flags
CC = gcc
AR = ar
CFLAGS = -g #-D __CHELPDEBUG__
CPPFLAGS = -std=gnu99 -Wall -Wextra -pedantic


# Setup dirs
BUILD_DIR = ./build
SRC_DIR = ./src
BIN_DIR = $(BUILD_DIR)/bin
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
INCLUDE_DIR = $(BUILD_DIR)/include


# Targets to make by default
LIBCHELPER = $(LIB_DIR)/libchelper.a


TARGETS = $(LIBCHELPER) libchelperinclude

# default rule and all
all: mkdirs $(TARGETS) libchelperinclude


# some target rules
libchelper: mkdirs $(LIBCHELPER) libchelperinclude
libchelperinclude:
	@mkdir -p $(INCLUDE_DIR)/chelper
	@cp $(CHELPER_DIR)/*.h $(INCLUDE_DIR)/chelper


# mkdirs and clean rules
mkdirs:
	@echo "making dirs"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(INCLUDE_DIR)
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

TEST_DIR = $(SRC_DIR)/test


# src lists
CHELPER_SOURCES = $(wildcard $(CHELPER_DIR)/*.c)

TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)


# obj lists
CHELPER_OBJS = $(patsubst $(CHELPER_DIR)/%.c, $(OBJ_DIR)/chelper_%.o, $(CHELPER_SOURCES))


# test binarys
TEST_BINS = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/test_%, $(TEST_SOURCES))
tests: mkdirs $(LIBCHELPER) libchelperinclude $(TEST_BINS)


# rules to build objects
$(OBJ_DIR)/chelper_%.o: $(CHELPER_DIR)/%.c
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	@echo "$(CC) $@"
	@$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) -I$(INCLUDE_DIR) $<


# linking rules
$(LIBCHELPER): $(CHELPER_OBJS)
	@echo "ar $@"
	@$(AR) rcs $@ $^
	@echo ""

$(BIN_DIR)/test_%: $(OBJ_DIR)/test_%.o
	@echo "$(CC) $@"
	@$(CC) -o $@ $(CFLAGS) -L$(LIB_DIR) $^ -lchelper
	@echo "finished $@"
	@echo ""
