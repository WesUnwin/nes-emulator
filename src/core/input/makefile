
INPUT_CORE_SOURCES := $(wildcard *.cpp)
INPUT_CORE_OBJECTS := $(patsubst %.cpp, obj/%.o, $(INPUT_CORE_SOURCES))

all: input_lib

message:
	@echo "-------------------------------------------------"
	@echo "     BUILDING INPUT SUBSYSTEM       "
	@echo "-------------------------------------------------"

obj_dir:
	@echo "=> Creating obj/ directory..."
	mkdir -p obj

obj/%.o: %.cpp message obj_dir
	g++ -w -c $< -o $@

input_lib: $(INPUT_CORE_OBJECTS)
	@echo "=> Linking libinput.a static library..."
	ar rvs libinput.a $(INPUT_CORE_OBJECTS)

clean:
	@echo "=> Deleting INPUT core objects and input core lib..."
	rm -rf obj
	rm -f libinput.a
