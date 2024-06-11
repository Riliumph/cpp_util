# compiler setting
CXXVER   = c++17
COMPILER = g++
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
LIBS    = -lstdc++
LIBS   += -lpthread -lpqxx

# include setting
INCLUDE = -I$(SRCROOT)/
EXCLUDE = .git%

# binary setting
TARGET  = $(shell basename $$(pwd))
BINROOT = bin

# source setting
SRCROOT = src
SRCS    = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRCROOT) -name "*.cpp"))
SRCDIRS = $(dir $(SRCS))

# object setting
OBJROOT = obj
OBJDIRS = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%, %, $(SRCDIRS)))
OBJS    = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%.cpp, %.o, $(SRCS)))

# test setting
TEST_EXT= %_test.cpp
TESTLIBS = $(LIBS) -lpthread -lgtest -lgtest_main
TESTROOT = $(SRCROOT)
TESTSRCS = $(filter-out $(EXCLUDE) %main.cpp, $(shell find $(SRCROOT) -name "*.cpp"))
TESTOBJS = $(addprefix $(OBJROOT)/, $(patsubst $(TESTROOT)/%.cpp, %.o, $(TESTSRCS)))
TESTDIRS = $(dir $(TESTSRCS))

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all debug release test analyze

all: debug

test: CXXFLAGS+= $(DEBUG_CXXFLAGS)
test: $(TESTOBJS)
	@echo "--- TEST MODE ---"
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) -o $(BINROOT)/$@ $^ $(LDFLAGS) $(TESTLIBS)
	$(BINROOT)/$@

debug: CXXFLAGS+= $(DEBUG_CXXFLAGS)
debug: $(TARGET)
	@echo "--- DEBUG MODE ---"

release: CXXFLAGS+= $(RELEASE_CXXFLAGS)
release: $(TARGET)
	@echo "--- RELEASE MODE ---"

$(TARGET): $(OBJS)
	@echo "build $< -> $@"
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) -o $(BINROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(OBJROOT)/%.o: $(SRCROOT)/%.cpp
	@echo "build $< -> $@"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

analyze:
	cppcheck --std=$(CXXVER) --enable=all $(SRCROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJROOT) $(BINROOT)

-include $(DEPS)
