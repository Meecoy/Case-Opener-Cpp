# =========[ Compiler settings ]=========
CXX      := g++
CXXFLAGS := -std=c++17 -Wall \
            -Ilibs/simdjson/include -Ilibs/simdjson/src \
            -Ilibs/raylib/src

# =========[ Sources ]=========
SRCS := main.cpp utilities.cpp libs/simdjson/src/simdjson.cpp
OBJS := $(SRCS:.cpp=.o)

# =========[ Paths ]=========
RAYLIB_DIR      := libs/raylib
RAYLIB_BUILD    := $(RAYLIB_DIR)/build
RAYLIB_LIB_PATH := $(RAYLIB_BUILD)/raylib
RAYLIB_LIB      := $(RAYLIB_LIB_PATH)/libraylib.a

# =========[ Flags ]=========
LDFLAGS := -L$(RAYLIB_LIB_PATH) -lraylib \
           -lGL -lm -lpthread -ldl -lrt -lX11

# =========[ Main goal ]=========
TARGET := CaseOpener.exe
.PHONY: all
all: $(TARGET)

# =========[ Binary ]=========
$(TARGET): $(OBJS) $(RAYLIB_LIB)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

# =========[ Compilation ]=========
%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# =========[ Building raylib ]=========
$(RAYLIB_LIB):
	@mkdir -p $(RAYLIB_BUILD)
	@cmake -S $(RAYLIB_DIR) -B $(RAYLIB_BUILD) \
	    -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF > /dev/null
	@$(MAKE) -C $(RAYLIB_BUILD) -s

# =========[ Cleanup ]=========
.PHONY: clean veryclean
clean:
	rm -f $(OBJS) $(TARGET)

veryclean: clean
	rm -rf $(RAYLIB_BUILD)

