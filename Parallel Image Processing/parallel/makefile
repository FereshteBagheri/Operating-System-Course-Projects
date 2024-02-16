
CXX = g++
CXXFLAGS = -Wall -std=c++11
OBJS = main.o bmp.o filters.o
TARGET = ImageFilters

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp bmp.hpp filters.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

bmp.o: bmp.cpp bmp.hpp
	$(CXX) $(CXXFLAGS) -c bmp.cpp

filters.o: filters.cpp filters.hpp bmp.hpp
	$(CXX) $(CXXFLAGS) -c filters.cpp

clean:
	rm -f $(TARGET) $(OBJS)
