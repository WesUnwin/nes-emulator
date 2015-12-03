
CPU_CORE_SOURCES := $(wildcard *.cpp)
CPU_CORE_OBJECTS := $(patsubst %.cpp, obj/%.o, $(CPU_CORE_SOURCES))

all: cpu_lib

message:
	@echo "-----------------------------"
	@echo "     BUILDING CPU CORE       "
	@echo "-----------------------------"

obj_dir:
	@echo "=> Creating obj/ directory..."
	mkdir -p obj

obj/%.o: %.cpp message obj_dir
	g++ -w -c $< -o $@

cpu_lib: $(CPU_CORE_OBJECTS)
	@echo "=> Linking libcpu.a static library..."
	ar rvs libcpu.a $(CPU_CORE_OBJECTS)

clean:
	@echo "=> Deleting CPU core objects and CPU core lib..."
	rm -rf obj
	rm -f libcpu.a
