# compiler setting
CXX      = g++
CXXVER   = c++23
CXXFLAGS = -MMD
CXXFLAGS+= -Wall -Wextra -Winit-self
CXXFLAGS+= -std=$(CXXVER)
# debug mode
DEBUG_CXXFLAGS = -g
DEBUG_CXXFLAGS+= -DDEBUG_
# release mode
RELEASE_CXXFLAGS = -O2

# library setting
LDFLAGS = -L/usr/local/lib
LDFLAGS+= -L./lib
LDLIBS  = -lstdc++
LDLIBS += -lspdlog
LDLIBS += -lfmt
LDLIBS += -lstd_ext
# include setting
INCLUDE = -I$(SRC_ROOT)/
EXCLUDE = .git%

# build settings
SRC_ROOT = src
OBJ_ROOT = obj

# install setting
HEAD_INS_PATH = /usr/local/include
LIB_INS_PATH  = /usr/local/lib

# bin target
BIN      = $(shell basename $$(pwd))
BIN_ROOT = bin
BIN_SRCS = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -mindepth 1 -maxdepth 1 -name "*.cpp"))
BIN_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(BIN_SRCS)))

# static library target
LIB      = lib$(BIN).a
LIB_ROOT = lib
LIB_SRCS = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -mindepth 2 -name "*.cpp"))
LIB_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(LIB_SRCS)))
LIB_HEADS= $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -mindepth 1 -name "*.h*"))
INS_LIB_HEADS= $(patsubst $(SRC_ROOT)/%, $(HEAD_INS_PATH)/%, $(LIB_HEADS))
INS_LIB  = $(LIB_INS_PATH)/$(LIB)

# test setting
TEST_LDLIBS = $(LDLIBS) -lpthread -lgtest -lgtest_main
TEST_EXT  = %_test.cpp
TEST_SRCS = $(filter-out $(EXCLUDE) %main.cpp, $(shell find $(SRC_ROOT) -name "*.cpp"))
TEST_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(TEST_SRCS)))

# dependency setting
OBJS = $(BIN_OBJS)
OBJS+= $(LIB_OBJS)
DEPS = $(OBJS:.o=.d)

.PHONY: clean all debug release test analyze install

# USER COMMANDS

all: debug

echo:
	@echo $(LIB_HEADS)
	@echo $(INS_LIB_HEADS)

test: CXXFLAGS+= $(DEBUG_CXXFLAGS)
test: $(TEST_OBJS)
	@echo "--- TEST MODE ---"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(CXX) -o $(BIN_ROOT)/$@ $^ $(LDFLAGS) $(TEST_LDLIBS)
#	$(BIN_ROOT)/$@ 自動実行するとVSCodeのテストデバッガが動かない

debug: CXXFLAGS+= $(DEBUG_CXXFLAGS)
debug: $(BIN)
	@echo "--- DEBUG MODE ---"

release: CXXFLAGS+= $(RELEASE_CXXFLAGS)
release: $(BIN)
	@echo "--- RELEASE MODE ---"

install: install_heads install_lib
	@echo "--- INSTALL COMPLETED ---"

analyze:
	cppcheck --std=$(CXXVER) --enable=all $(SRC_ROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJ_ROOT) $(BIN_ROOT) $(LIB_ROOT)

# BUID RULES

# binary build rule
$(BIN): $(LIB) $(BIN_OBJS)
	@echo "build bin $^ -> $@"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(CXX) -o $(BIN_ROOT)/$@ $(BIN_OBJS) $(LDFLAGS) $(LDLIBS) -l$(BIN)

# static library build rule
$(LIB): $(LIB_OBJS)
	@echo "build lib $^ -> $@"
	@[ -d $(LIB_ROOT) ] || mkdir -p $(LIB_ROOT)
	ar rcv $(LIB_ROOT)/$@ $^

# object build rule
$(OBJ_ROOT)/%.o: $(SRC_ROOT)/%.cpp
	@echo "build obj $< -> $@"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

# INSTALL RULES

install_heads: $(INS_LIB_HEADS)
	@echo "--- HEADERS INSTALL COMPLETED ---"

# header copy rule
$(HEAD_INS_PATH)/%: $(SRC_ROOT)/%
	@[ -d $@ ] || mkdir -p $(dir $@)
	@install -v -m 644 $< $@

install_lib: $(INS_LIB)
	@echo "--- LIBS INSTALL COMPLETED ---"

# lib copy rule
$(LIB_INS_PATH)/%: $(LIB_ROOT)/%
	@[ -d $@ ] || mkdir -p $(dir $@)
	@install -v -m 644 $< $@

-include $(DEPS)
