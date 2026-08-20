# compiler setting
COMPILER = gcc -v
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++

# include setting
INCLUDE = -I$(SRC_ROOT)/
EXCLUDE = .git%
TEST_EXT= %_test.c

# binary setting
TARGET   = $(shell basename $$(pwd)).ko
BIN_ROOT = bin

# source setting
SRC_ROOT = src
SRCS     = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -name "*.c"))
SRC_DIRS = $(dir $(SRCS))

# object setting
OBJ_ROOT = obj
OBJ_DIRS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%, %, $(SRC_DIRS)))
OBJS     = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.c, %.o, $(SRCS)))

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all analyze install uninstall

all: $(OBJS) $(TARGET)
	@echo "[make all]"

install:
	insmod $(TARGET)

uninstall:
	rmmod $(TARGET)

$(TARGET): $(OBJS)
	@echo "[build $< -> $@]"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(COMPILER) -o $(BIN_ROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(OBJ_ROOT)/%.o: $(SRC_ROOT)/%$(SRC_EXT)
	@echo "[build $< -> $@]"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

analyze:
	cppcheck --std=c++17 --enable=all $(SRC_ROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJ_ROOT) $(BIN_ROOT)

-include $(DEPS)
