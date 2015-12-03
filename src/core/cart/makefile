
CART_CORE_SOURCES := $(wildcard *.cpp)
CART_CORE_OBJECTS := $(patsubst %.cpp, obj/%.o, $(CART_CORE_SOURCES))

all: cart_lib

message:
	@echo "-------------------------------------------------"
	@echo "     BUILDING CARTRIDGE & MAPPER SUBSYSTEM       "
	@echo "-------------------------------------------------"

obj_dir:
	@echo "=> Creating obj/ directory..."
	mkdir -p obj

obj/%.o: %.cpp message obj_dir
	g++ -w -c $< -o $@

cart_lib: $(CART_CORE_OBJECTS)
	@echo "=> Linking libcart.a static library..."
	ar rvs libcart.a $(CART_CORE_OBJECTS)

clean:
	@echo "=> Deleting CART core objects and cart core lib..."
	rm -rf obj
	rm -f libcart.a
