CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -g
LDFLAGS  := 

SRCS := \
    mp1.cpp \
    cs225/HSLAPixel.cpp \
    cs225/PNG.cpp \
    cs225/lodepng/lodepng.cpp

OBJS := $(SRCS:.cpp=.o)
TARGET := mp1

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

