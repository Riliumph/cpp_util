# compiler setting
COMPILER = g++ -v
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self -std=c++17

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++
LIBS   += -lhiredis
LIBS   += -lredis++

# include setting
INCLUDE = -I$(SRC_ROOT)/
EXCLUDE = .git%

# binary setting
TARGET   = $(shell basename $$(pwd))
BIN_ROOT = bin

# source setting
SRC_ROOT = src
SRCS     = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -name "*.cpp"))
SRC_DIRS = $(dir $(SRCS))

# object setting
OBJ_ROOT = obj
OBJ_DIRS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%, %, $(SRC_DIRS)))
OBJS     = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(SRCS)))

# test setting
TEST_EXT  = %_test.cpp
TEST_LIBS = $(LIBS) -lpthread -lgtest -lgtest_main
TEST_ROOT = src
TEST_SRCS = $(filter-out $(EXCLUDE) %main.cpp, $(shell find $(SRC_ROOT) -name "*.cpp"))
TEST_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(TEST_ROOT)/%.cpp, %.o, $(TEST_SRCS)))
TEST_DIRS = $(dir $(TEST_SRCS))

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all analyze

all: $(OBJS) $(TARGET)
	@echo "make all"

test: $(TEST_OBJS)
	@echo "build $< -> $@"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(COMPILER) -o $(BIN_ROOT)/$@ $^ $(LDFLAGS) $(TEST_LIBS)

$(TARGET): $(OBJS)
	@echo "build $< -> $@"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(COMPILER) -o $(BIN_ROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(OBJ_ROOT)/%.o: $(SRC_ROOT)/%.cpp
	@echo "build $< -> $@"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

analyze:
	cppcheck --std=c++17 --enable=all $(SRC_ROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJ_ROOT) $(BIN_ROOT)

-include $(DEPS)
