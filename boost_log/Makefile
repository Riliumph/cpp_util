# compiler setting
CXXVER   = c++17
COMPILER = g++
CXXFLAGS = -MMD
CXXFLAGS+= -Wall -Wextra -Winit-self
CXXFLAGS+= -std=$(CXXVER)
CXXFLAGS+= -DBOOST_LOG_DYN_LINK
# debug mode
DEBUG_CXXFLAGS = -g
DEBUG_CXXFLAGS+= -DDEBUG_
# release mode
RELEASE_CXXFLAGS = -O2

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++
LIBS   += -lpthread
LIBS   += -lboost_thread -lboost_log

# include setting
INCLUDE = -I$(SRC_ROOT)/
EXCLUDE = .git%

# binary setting
TARGET  = $(shell basename $$(pwd))
BIN_ROOT = bin

# source setting
SRC_ROOT = src
SRCS    = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -name "*.cpp"))
SRC_DIRS = $(dir $(SRCS))

# object setting
OBJ_ROOT = obj
OBJ_DIRS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%, %, $(SRC_DIRS)))
OBJS    = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(SRCS)))

# test setting
TEST_EXT= %_test.cpp
TEST_LIBS = $(LIBS) -lpthread -lgtest -lgtest_main
TEST_ROOT = $(SRC_ROOT)
TEST_SRCS = $(filter-out $(EXCLUDE) %main.cpp, $(shell find $(SRC_ROOT) -name "*.cpp"))
TEST_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(TEST_ROOT)/%.cpp, %.o, $(TEST_SRCS)))
TEST_DIRS = $(dir $(TEST_SRCS))

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all debug release test analyze

all: debug

test: CXXFLAGS+= $(DEBUG_CXXFLAGS)
test: $(TEST_OBJS)
	@echo "--- TEST MODE ---"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(COMPILER) -o $(BIN_ROOT)/$@ $^ $(LDFLAGS) $(TEST_LIBS)
	$(BIN_ROOT)/$@

debug: CXXFLAGS+= $(DEBUG_CXXFLAGS)
debug: $(TARGET)
	@echo "--- DEBUG MODE ---"

release: CXXFLAGS+= $(RELEASE_CXXFLAGS)
release: $(TARGET)
	@echo "--- RELEASE MODE ---"

$(TARGET): $(OBJS)
	@echo "build $< -> $@"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(COMPILER) -o $(BIN_ROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(OBJ_ROOT)/%.o: $(SRC_ROOT)/%.cpp
	@echo "build $< -> $@"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

analyze:
	cppcheck --std=$(CXXVER) --enable=all $(SRC_ROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJ_ROOT) $(BIN_ROOT)

-include $(DEPS)
