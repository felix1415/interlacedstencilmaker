CXXFLAGS += -std=c++11 -Wall -Wextra -Iinclude

SOURCES := main.cpp $(shell find src/ -name '*.cpp')
HEADERS := $(shell find . -name '*.h')

OUTPUT := stencil_maker

all: $(OUTPUT)

$(OUTPUT): $(SOURCES) $(HEADERS) $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(OUTPUT) $(SOURCES)

clean:
	$(RM) $(OUTPUT)