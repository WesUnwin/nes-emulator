
APU_CORE_SOURCES := $(wildcard *.cpp)
APU_CORE_OBJECTS := $(patsubst %.cpp, obj/%.o, $(APU_CORE_SOURCES))

all: apu_lib

message:
	@echo "-----------------------------"
	@echo "     BUILDING APU CORE       "
	@echo "-----------------------------"

obj_dir:
	@echo "=> Creating obj/ directory..."
	mkdir -p obj

obj/%.o: %.cpp message obj_dir
	g++ -w -c $< -o $@

apu_lib: $(APU_CORE_OBJECTS)
	@echo "=> Linking libapu.a static library..."
	ar rvs libapu.a $(APU_CORE_OBJECTS)

clean:
	@echo "=> Deleting APU core objects and APU core lib..."
	rm -rf obj
	rm -f libapu.a
