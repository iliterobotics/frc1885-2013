# Basic makefile.
PRJROOT := ../..
CPPSRC := $(PRJROOT)/common/SocketUtils.cpp $(wildcard $(PRJROOT)/comm/cpp/*.cpp) $(wildcard *.cpp) $(wildcard $(PRJROOT)/comm/cpp/tests/*.cpp)
OBJFILES := $(CPPSRC:.cpp=.o)
TARGET := chunk-test
CXX := g++
CXXFLAGS += -O0 -ggdb -std=c++11

test: $(TARGET)
	./chunk-test

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(OBJFILES)

clean:
	@rm -f $(TARGET) $(OBJFILES)
