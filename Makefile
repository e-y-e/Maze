TARGET := maze

BUILD_DIR ?= ./build
PROF_DIR ?= ./prof

# Define the output files in terms of the input files.
SRCS := location.c action.c maze_size.c node_list.c maze.c io.c main.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

INC_DIRS := include
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

# Define compiler & linker flags.
CC := clang
CFLAGS := -std=c11 -Weverything -Wno-documentation-unknown-command -MMD -MP $(INC_FLAGS)
LDFLAGS := -fuse-ld=lld

OPT_CFLAGS := -DNDEBUG -flto -O2 -Rpass=.* -Rpass-missed=.* -Rpass-analysis=.*


.PHONY: debug profile release release-pgo clean


# Define additional flags for debug build.
debug: CFLAGS += -Werror -O0 -glldb
debug: $(BUILD_DIR)/$(TARGET)

# Define additional flags for profile build.
profile: CFLAGS += $(OPT_CFLAGS) -fprofile-instr-generate="$(PROF_DIR)/%p.profraw"
profile: LDFLAGS +=  -fprofile-instr-generate="$(PROF_DIR)/%p.profraw"
profile: $(BUILD_DIR)/$(TARGET)

# Define additional flags for release build.
release: CFLAGS += $(OPT_CFLAGS)
release: $(BUILD_DIR)/$(TARGET)

# Define additional flags for release-pgo (release with profile guided optimization) build.
release-pgo: CFLAGS += $(OPT_CFLAGS) -fprofile-instr-use=$(BUILD_DIR)/$(TARGET).profdata
release-pgo: $(BUILD_DIR)/$(TARGET).profdata $(BUILD_DIR)/$(TARGET)

clean:
	$(RM) $(BUILD_DIR)/*


$(BUILD_DIR)/$(TARGET): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).profdata:
	llvm-profdata merge -output=$@ $(PROF_DIR)

-include $(DEPS)
