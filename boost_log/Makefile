# compiler setting
COMPILER = g++ -v
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self -std=c++17 -DBOOST_LOG_DYN_LINK

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++ -lpthread -lboost_thread -lboost_log

# include setting
INCLUDE = -I$(SRCROOT)
EXCLUDE = .git%

# binary setting
TARGET  = $(shell basename $$(pwd))
BINROOT = bin

# source setting
SRCROOT = src
SRCS    = $(filter-out $(EXCLUDE), $(shell find $(SRCROOT) -name "*.cpp"))
SRCDIRS = $(dir $(SRCS))

# object setting
OBJROOT = obj
OBJDIRS = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%, %, $(SRCDIRS)))
OBJS    = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%.cpp, %.o, $(SRCS)))

# test setting
TESTLIBS = $(LIBS) -lpthread -lgtest -lgtest_main
TESTROOT = src
TESTSRCS = $(filter-out $(EXCLUDE), $(shell find $(SRCROOT) -name "*_test.cpp"))
TESTOBJS = $(addprefix $(OBJROOT)/, $(patsubst $(TESTROOT)/%.cpp, %.o, $(TESTSRCS)))
TESTDIRS = $(dir $(TESTSRCS))

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all analyze

analyze:
	cppcheck --std=c++17 --enable=all $(SRCROOT)

all: $(OBJS) $(TARGET)
	@echo "make all"

$(TARGET): $(OBJS)
	@echo "build exe $@"
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) $(INCLUDE) -o $(BINROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(OBJROOT)/%.o: $(SRCROOT)/%.cpp
	@echo "build $@ from $<"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

analyze:
	cppcheck --std=c++17 --enable=all $(SRCROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJROOT) $(BINROOT)

-include $(DEPS)
