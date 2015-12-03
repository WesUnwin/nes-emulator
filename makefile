TOP := $(realpath ../)

CC = g++

CFLAGS = -Wall

CPU_CORE_SOURCES := $(wildcard $(TOP)/src/cpu/*.cpp)
OBJECTS := $(addprefix $(TOP)/project/obj,$(patsubst $(TOP),,$(SOURCES:%.cpp=%.o)))

$(OBJECTS): $(TOP)/project/obj/%.o: $(TOP)/%.cpp
	$(CC) $(CFLAGS) -c -o $@

abc:
	@echo $(realpath .)/src


clean:
	@echo "Cleaning up..."
	rm abc.txt
