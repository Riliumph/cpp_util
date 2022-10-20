# compiler setting
COMPILER  = g++ -v
CXXFLAGS  = -g -MMD -Wall -Wextra -Winit-self -std=c++17

# library setting
ifeq "$(shell getconf LONG_BIT)" "64"
  LDFLAGS = -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu
else
  LDFLAGS = -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu
endif
LIBS      = -lstdc++ -lboost_log -lpthread

# include setting
INCLUDE   = -I/usr/include -I./ 
EXCLUDE   = .git%

# binary setting
TARGET  = $(shell basename $$(readlink -f .))
BINROOT = bin
BIN     = $(BINDIR)/$(TARGET)

# source setting
SRCROOT   = .
SRCS      = $(filter-out $(NOMAKEDIR), $(shell find ./ -name "*.cpp"))
SRCDIRS   = $(dir $(SRCS))

# object setting
OBJROOT   = obj
OBJDIRS   = $(addprefix $(OBJROOT)/, $(SRCDIRS))
OBJS      = $(addprefix $(OBJROOT)/, $(patsubst %.cpp, %.o, $(SRCS)))

# dependency setting
DEPS      = $(OBJS:.o=.d)

default:
	@make all

all: $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	@[ -d $(BINROOT) ] || mkdir -p $(BINROOT)
	$(COMPILER) -o $(BINROOT)/$@ $^ $(LDFLAGS) $(LIBS)

$(OBJROOT)/%.o: %.cpp
	@echo $@
	@echo $^
	@[ -d $@ ] || mkdir -p $(dir $@)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

fix_lib:
	@ldconfig

clean:
	@rm -rf $(OBJROOT) $(BINROOT)

-include $(DEPS)
