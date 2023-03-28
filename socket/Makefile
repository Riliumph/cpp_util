# compiler setting
COMPILER = g++ -v
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self -std=c++17

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++

# include setting
INCLUDE = -I$(SRCROOT)/
EXCLUDE = .git%
TEST_EXT= %_test.cpp

# binary setting
SERVER  = server
CLIENT  =
BINROOT = bin

# source setting
SRCROOT = src
SRCS    = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRCROOT) -name "*.cpp"))
SRCDIRS = $(dir $(SRCS))

# object setting
OBJROOT = obj
OBJDIRS = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%, %, $(SRCDIRS)))
OBJS    = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%.cpp, %.o, $(SRCS)))
SERVER_OBJS = $(OBJS)

# test setting
TESTEXCLUDE = $(shell grep -lr --include="*.cpp" "main" $(SRCROOT))
TESTROOT = src
TESTS    = $(filter-out $(EXCLUDE) $(TESTEXCLUDE), $(shell find $(SRCROOT) -name "*.cpp"))
TESTDIRS = $(dir $(SRCS))
TESTLIBS = $(LIBS) -lpthread -lgtest -lgtest_main

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all

echo:
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(SERVER_OBJS)

test:
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) $(TESTS) $(CXXFLAGS) $(LDFLAGS) $(TESTLIBS) $(INCLUDE) -o $(BINROOT)/test

all: $(OBJS) $(SERVER)
	@echo "make all"

$(SERVER): $(SERVER_OBJS)
	@echo "build exe $@"
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) -o $(BINROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(OBJROOT)/%.o: $(SRCROOT)/%.cpp
	@echo "build $@ from $<"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJROOT) $(BINROOT)

-include $(DEPS)
