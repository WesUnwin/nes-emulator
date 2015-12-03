
PPU_CORE_SOURCES := $(wildcard *.cpp)
PPU_CORE_OBJECTS := $(patsubst %.cpp, obj/%.o, $(PPU_CORE_SOURCES))

all: ppu_lib

message:
	@echo "-----------------------------"
	@echo "     BUILDING PPU CORE       "
	@echo "-----------------------------"

obj_dir:
	@echo "=> Creating obj/ directory..."
	mkdir -p obj

obj/%.o: %.cpp message obj_dir
	g++ -w -c $< -o $@

ppu_lib: $(PPU_CORE_OBJECTS)
	@echo "=> Linking libppu.a static library..."
	ar rvs libppu.a $(PPU_CORE_OBJECTS)

clean:
	@echo "=> Deleting PPU core objects and PPU core lib..."
	rm -rf obj
	rm -f libppu.a
