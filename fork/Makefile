# compiler setting
COMPILER = g++ -v
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self -std=c++17

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++ -lpthread

# include setting
INCLUDE = -I$(SRCROOT)/
EXCLUDE = .git%

# binary setting
TARGET1  = $(shell basename $$(pwd))
TARGET2  = $(shell basename $$(pwd))_sub
BINROOT = bin

# source setting
SRCROOT = src
SRCS    = $(filter-out $(EXCLUDE), $(shell find $(SRCROOT) -name "*.cpp"))
SRCDIRS = $(dir $(SRCS))

# object setting
OBJROOT = obj
OBJDIRS = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%, %, $(SRCDIRS)))
OBJS    = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%.cpp, %.o, $(SRCS)))
TARGET1_OBJ = $(filter-out %/sub.o, $(OBJS))
TARGET2_OBJ = $(filter-out %/main.o, $(OBJS))

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

all: $(TARGET1) $(TARGET2)
	@echo "make all"

$(TARGET1): $(TARGET1_OBJ)
	@echo "build exe $@"
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) -o $(BINROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(TARGET2): $(TARGET2_OBJ)
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
