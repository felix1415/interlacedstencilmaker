# set non-optional compiler flags here
CXXFLAGS += -std=c++11 -Wall -Wextra -Iinclude

SOURCES := $(shell find . -name '*.cpp')
HEADERS := $(shell find . -name '*.h')

OUTPUT := stencil_maker

all: $(OUTPUT)

$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(OUTPUT) $(SOURCES)

clean:
	$(RM) $(OUTPUT)