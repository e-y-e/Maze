TARGET := maze

BUILD_DIR ?= ./build
PROF_DIR ?= ./prof

# Define the output files in terms of the input files.
SRCS := location.c maze_size.c node_list.c maze.c io.c main.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

INC_DIRS := include
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

# Define compiler & linker flags.
CC := clang
CFLAGS := -std=c11 -Weverything -MMD -MP $(INC_FLAGS)
LDFLAGS := -fuse-ld=lld

OPT_CFLAGS := -flto -O2 -Rpass=.* -Rpass-missed=.* -Rpass-analysis=.*


.PHONY: debug test profile release clean


# Define additional flags for debug build.
debug: CFLAGS += -Werror -O0 -glldb
debug: $(BUILD_DIR)/$(TARGET)

# Define additional flags for test build.
test: CFLAGS += -DTEST -Werror -O0 -glldb
test: $(BUILD_DIR)/$(TARGET)

# Define additional flags for profile build.
profile: CFLAGS += $(OPT_CFLAGS) -fprofile-instr-generate="$(PROF_DIR)/%p.profraw"
profile: LDFLAGS += -fprofile-instr-generate="$(PROF_DIR)/%p.profraw"
profile: $(PROF_DIR) $(BUILD_DIR)/$(TARGET)

# Define additional flags for release build.
release: CFLAGS += $(OPT_CFLAGS) -fprofile-instr-use=$(BUILD_DIR)/$(TARGET).profdata
release: $(BUILD_DIR)/$(TARGET).profdata $(BUILD_DIR)/$(TARGET)

clean:
	$(RM) $(BUILD_DIR)/*


$(BUILD_DIR)/$(TARGET): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).profdata: $(PROF_DIR)
	llvm-profdata merge -output=$@ $(PROF_DIR)

$(PROF_DIR):
	mkdir -p $(PROF_DIR)


-include $(DEPS)
