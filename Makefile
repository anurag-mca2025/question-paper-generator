CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Iinclude
TARGET = question_generator
SOURCES = src/main.cpp src/file_manager.cpp src/preprocessor.cpp src/keyword_extractor.cpp \
          src/line_scorer.cpp src/question_generator.cpp src/paper_exporter.cpp src/ui.cpp
OBJECTS = $(SOURCES:src/%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean

