
all: message cpu_core ppu_core apu_core cart_core input_core emu

message:
	@echo "                                               "
	@echo "=========== BUILDING EMULATOR CORE ============"
	@echo "                                               "

cpu_core:
	cd cpu && $(MAKE)

ppu_core:
	cd ppu && $(MAKE)

apu_core:
	cd apu && $(MAKE)

cart_core:
	cd cart && $(MAKE)

input_core:
	cd input && $(MAKE)

emu:
	g++ -Wall -w -c Emu.cpp -o emu.o

clean:
	cd cpu && $(MAKE) clean
	cd ppu && $(MAKE) clean
	cd apu && $(MAKE) clean
	cd cart && $(MAKE) clean
	cd input && $(MAKE) clean

