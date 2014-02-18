# Basic makefile.
PRJROOT := ../..
CPPSRC := $(PRJROOT)/common/SocketUtils.cpp $(PRJROOT)/common/Communicator.cpp $(wildcard $(PRJROOT)/comm/cpp/*.cpp) $(wildcard *.cpp) $(wildcard $(PRJROOT)/comm/cpp/tests/*.cpp)
OBJFILES := $(CPPSRC:.cpp=.o)
TARGET := CommunicatorTest
CXX := g++
CXXFLAGS := -O0 -ggdb -std=c++0x
CXXLINKS := -lpthread

test: $(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(OBJFILES) $(CXXLINKS)

clean:
	@rm -f $(TARGET) $(OBJFILES)
