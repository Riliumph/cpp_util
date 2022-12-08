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

# dependency setting
DEPS = $(OBJS:.o=.d)

.PHONY: clean all

echo:
	@echo $(TARGET1)
	@echo $(TARGET1_OBJ)
	@echo $(TARGET2)
	@echo $(TARGET2_OBJ)

all: $(OBJS) $(TARGET1) $(TARGET2)
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
