# compiler setting
CXX      = g++
CXXVER   = c++17
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
# include setting
INCLUDE = -I$(SRC_ROOT)/
EXCLUDE = .git%

# build settings
SRC_ROOT = src
OBJ_ROOT = obj

# install setting
HEAD_INS_PATH = /usr/local/include
LIB_INS_PATH  = /usr/local/lib

# project
PRJ = $(shell basename $$(pwd))

# static library target
LIB      = lib$(PRJ).so
LIB_ROOT = lib
LIB_SRCS = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -mindepth 2 -name "*.cpp"))
LIB_OBJS = $(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(LIB_SRCS)))
LIB_HEADS= $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -mindepth 1 -name "*.h*"))
INS_LIB_HEADS= $(patsubst $(SRC_ROOT)/%, $(HEAD_INS_PATH)/%, $(LIB_HEADS))
INS_LIB  = $(LIB_INS_PATH)/$(LIB)

# dependency setting
OBJS = $(LIB_OBJS)
DEPS = $(OBJS:.o=.d)

.PHONY: clean all debug release test analyze install

# USER COMMANDS

all: debug

echo:
	@echo $(LIB_HEADS)
	@echo $(INS_LIB_HEADS)

debug: CXXFLAGS+= $(DEBUG_CXXFLAGS)
debug: $(LIB)
	@echo "--- DEBUG MODE ---"

release: CXXFLAGS+= $(RELEASE_CXXFLAGS)
release: $(LIB)
	@echo "--- RELEASE MODE ---"

install: install_heads install_lib
	@echo "--- INSTALL COMPLETED ---"

analyze:
	cppcheck --std=$(CXXVER) --enable=all $(SRC_ROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJ_ROOT) $(LIB_ROOT)

# BUILD RULES

# dynamic library build rule
$(LIB): $(LIB_OBJS)
	@echo "build lib $^ -> $@"
	@[ -d $(LIB_ROOT) ] || mkdir -p $(LIB_ROOT)
	$(CXX) $(CXXFLAGS) -shared -o $(LIB_ROOT)/$@ $^ $(LDFLAGS) $(LIBS)

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
