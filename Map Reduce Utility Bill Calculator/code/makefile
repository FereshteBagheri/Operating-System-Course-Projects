CXX = g++
CXXFLAGS = -std=c++17
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
EXES := $(basename $(SRCS))

.PHONY: all clean

all: $(EXES)

%: %.o
	$(CXX) $(CXXFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXES)
