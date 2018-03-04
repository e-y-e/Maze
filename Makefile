TARGET ?= maze

BUILD_DIR ?= ./build

# Define the output files in terms of the input files.
SRCS := main.c location.c maze_size.c node_list.c maze.c io.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

INC_DIRS := include
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

# Define compiler & linker flags.
CC := clang
CFLAGS := -std=c11 -Weverything -MMD -MP $(INC_FLAGS)
LDFLAGS := -fuse-ld=lld


.PHONY: debug release clean debug_extra release_extra


# Define additional flags for debug build.
debug: CFLAGS += -Werror -O0 -g
debug: $(BUILD_DIR)/$(TARGET)

# Define additional flags for release build.
release: CFLAGS += -flto -O2 -Rpass=.* -Rpass-missed=.* -Rpass-analysis=.*
release: $(BUILD_DIR)/$(TARGET)

clean:
	$(RM) $(BUILD_DIR)/*


$(BUILD_DIR)/$(TARGET): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


-include $(DEPS)
