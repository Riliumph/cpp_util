# compiler setting
CXXVER   = c++17
COMPILER = g++
CXXFLAGS = -MMD
CXXFLAGS+= -Wall -Wextra -Winit-self
CXXFLAGS+= -std=$(CXXVER)
CXXFLAGS+= -fPIC
# debug mode
DEBUG_CXXFLAGS = -g
DEBUG_CXXFLAGS+= -DDEBUG_
# release mode
RELEASE_CXXFLAGS = -O2

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++
LIBS   += -lchrono
LIBS   += -lpthread -lpqxx
LDFLAGS+= -L./lib
# include setting
INCLUDE = -I$(SRC_ROOT)/
INCLUDE+= -I../chrono/src/
INCLUDE+= -I../variant/src/
EXCLUDE = .git%

# build settings
SRC_ROOT = src
OBJ_ROOT = obj

# bin target
BIN      = $(shell basename $$(pwd))
BIN_ROOT = bin
BIN_SRCS = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -mindepth 1 -maxdepth 1 -name "*.cpp"))
BIN_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(BIN_SRCS)))

# static library target
LIB      = lib$(BIN).so
LIB_ROOT = lib
LIB_SRCS = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -mindepth 2 -name "*.cpp"))
LIB_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(LIB_SRCS)))

# test setting
TEST_LIBS = $(LIBS) -lpthread -lgtest -lgtest_main
TEST_EXT  = %_test.cpp
TEST_SRCS = $(filter-out $(EXCLUDE) %main.cpp, $(shell find $(SRC_ROOT) -name "*.cpp"))
TEST_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(TEST_SRCS)))

# dependency setting
OBJS = $(BIN_OBJS)
OBJS+= $(LIB_OBJS)
DEPS = $(OBJS:.o=.d)

.PHONY: clean all debug release test analyze

all: debug

test: CXXFLAGS+= $(DEBUG_CXXFLAGS)
test: $(TEST_OBJS)
	@echo "--- TEST MODE ---"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(COMPILER) -o $(BIN_ROOT)/$@ $^ $(LDFLAGS) $(TEST_LIBS)

debug: CXXFLAGS+= $(DEBUG_CXXFLAGS)
debug: $(BIN)
	@echo "--- DEBUG MODE ---"

release: CXXFLAGS+= $(RELEASE_CXXFLAGS)
release: $(BIN)
	@echo "--- RELEASE MODE ---"

install:
	@echo "--- INSTALL ---"
	@mkdir -p /etc/cpp_util/postgres/lib
	@cp bin/postgres /etc/cpp_util/postgres
	@cp -rf lib /etc/cpp_util/postgres

# binary build rule
$(BIN): $(LIB) $(BIN_OBJS)
	@echo "build bin $^ -> $@"
	@[ -d $(BIN_ROOT) ] || mkdir -p $(BIN_ROOT)
	$(COMPILER) -o $(BIN_ROOT)/$@ $(BIN_OBJS) $(LDFLAGS) $(LIBS) -Wl,-rpath=./lib/ -l$(BIN)

# static library build rule
$(LIB): $(LIB_OBJS)
	@echo "build lib $^ -> $@"
	@[ -d $(LIB_ROOT) ] || mkdir -p $(LIB_ROOT)
	$(COMPILER) $(CXXFLAGS) -shared -o $(LIB_ROOT)/$@ $^ $(LDFLAGS) $(LIBS)

# object build rule
$(OBJ_ROOT)/%.o: $(SRC_ROOT)/%.cpp
	@echo "build obj $< -> $@"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

analyze:
	cppcheck --std=$(CXXVER) --enable=all $(SRC_ROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJ_ROOT) $(BIN_ROOT) $(LIB_ROOT)

-include $(DEPS)
