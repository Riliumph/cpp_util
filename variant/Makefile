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
TESTROOT = src
TESTS    = $(filter-out $(EXCLUDE), $(shell find $(SRCROOT) -name "*_test.cpp"))
TESTDIRS = $(dir $(SRCS))
TESTLIBS = $(LIBS) -lpthread -lgtest -lgtest_main

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all

echo:
	@echo $(SRCS)
	@echo $(OBJS)

test:
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) $(TESTS) $(CXXFLAGS) $(LDFLAGS) $(TESTLIBS) $(INCLUDE) -o $(BINROOT)/test
	$(BINROOT)/test

all: $(OBJS) $(TARGET)
	@echo "make all"

$(TARGET): $(OBJS)
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
