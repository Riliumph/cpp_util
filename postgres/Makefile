# compiler setting
COMPILER = g++ -v
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self -std=c++17

# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++ -pthread -lpqxx

# include setting
INCLUDE = -I/usr/local/include/ -I./
EXCLUDE = .git%

# binary setting
TARGET  = $(shell basename $$(pwd))
BINROOT = bin
BIN     = $(BINDIR)/$(TARGET)

# source setting
SRCROOT = .
SRCS    = $(filter-out $(EXCLUDE), $(shell find ./ -name "*.cpp"))
SRCDIRS = $(dir $(SRCS))

# object setting
OBJROOT = obj
OBJDIRS = $(addprefix $(OBJROOT)/, $(SRCDIRS))
OBJS    = $(addprefix $(OBJROOT)/, $(patsubst %.cpp, %.o, $(SRCS)))

# dependency setting
DEPS = $(OBJS:.o=.d)

default:
	@make all

all: $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) $(CXXFLAGS) -o $(BINROOT)/$@ $^ $(LDFLAGS) $(LIBS) $(INCLUDE)

$(OBJROOT)/%.o: %.cpp
	@echo %.cpp
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJROOT) $(BINROOT)

-include $(DEPS)
