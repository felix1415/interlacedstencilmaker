CXXFLAGS += -std=c++11 -Wall -g -Wextra -I../include -Iinclude

TESTS := $(shell find . -name '*.cpp') 
SOURCES := $(shell find ../src -name '*.cpp') 

OUTPUT := ism_test

all: $(OUTPUT)

$(OUTPUT): $(SOURCES) $(TESTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(OUTPUT) $(SOURCES) $(TESTS)

clean:
	$(RM) $(OUTPUT)
