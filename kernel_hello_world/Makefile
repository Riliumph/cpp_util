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
SRCROOT = src
SRCS    = $(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRCROOT) -name "*.c"))
SRCDIRS = $(dir $(SRCS))

# object setting
OBJROOT = obj
OBJDIRS = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%, %, $(SRCDIRS)))
OBJS    = $(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%.c, %.o, $(SRCS)))

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all analyze install uninstall

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

analyze:
	cppcheck --std=c++17 --enable=all $(SRCROOT)

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJROOT) $(BINROOT)

-include $(DEPS)
