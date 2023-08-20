# compiler setting
COMPILER = gcc -v
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++

# include setting
INCLUDE = -I$(SRCROOT)/
EXCLUDE = .git%
TEST_EXT= %_test.c

# binary setting
TARGET  = $(shell basename $$(pwd)).ko
BINROOT = bin

# source setting
SRCEXT  = .c
SRCROOT = src
SRCS    = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRCROOT) -name "*$(SRCEXT)"))
SRCDIRS = $(dir $(SRCS))

# object setting
OBJROOT = obj
OBJDIRS = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%, %, $(SRCDIRS)))
OBJS    = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%$(SRCEXT), %.o, $(SRCS)))

# test setting
TESTLIBS = $(LIBS) -lpthread -lgtest -lgtest_main
TESTROOT = src
TESTSRCS = $(filter-out $(EXCLUDE), $(shell find $(SRCROOT) -name "*_test$(SRCEXT)"))
TESTOBJS = $(addprefix $(OBJROOT)/, $(patsubst $(TESTROOT)/%$(SRCEXT), %.o, $(TESTSRCS)))
TESTDIRS = $(dir $(TESTSRCS))

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all install uninstall

echo:
	@echo "SRCS: $(SRCDIRS)"
	@echo "$(SRCROOT)/%.cpp"
	@echo "OBJS: $(OBJS)"

test:
	@echo "[make test]"
	@echo "[build $< -> $@]"
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) $(TESTSRCS) $(CXXFLAGS) $(LDFLAGS) $(TESTLIBS) $(INCLUDE) -o $(BINROOT)/$@
	$(BINROOT)/$@

all: $(OBJS) $(TARGET)
	@echo "[make all]"

install:
	insmod $(TARGET)

uninstsall:
	rmmod $(TARGET)

$(TARGET): $(OBJS)
	@echo "[build $< -> $@]"
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) -o $(BINROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(OBJROOT)/%.o: $(SRCROOT)/%$(SRCEXT)
	@echo "[build $< -> $@]"
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJROOT) $(BINROOT)

-include $(DEPS)