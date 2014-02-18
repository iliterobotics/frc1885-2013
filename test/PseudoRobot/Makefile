# Basic makefile.
PRJROOT := ../..
CPPSRC := $(PRJROOT)/common/SocketUtils.cpp $(PRJROOT)/common/Communicator.cpp $(wildcard $(PRJROOT)/comm/cpp/*.cpp) $(wildcard *.cpp) $(wildcard $(PRJROOT)/comm/cpp/tests/*.cpp)
OBJFILES := $(CPPSRC:.cpp=.o)
TARGET := PseudoRobot
CXX := g++
CXXFLAGS += -O0 -ggdb -std=c++0x

test: $(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o $@ $(OBJFILES)

clean:
	@rm -f $(TARGET) $(OBJFILES)
