CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = 

SRCS = main.cpp lexer.cpp symbol_table.cpp error_handler.cpp grammar.cpp parser.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = compiler

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f tokens.txt token_stream.txt errors.txt error.txt
	rm -f first_follow.txt parse_table.txt parsing_stages.txt
	mkdir -p output  # Ensure directory exists
	rm -f output/*.txt  # Remove only txt files in output directory

# Dependencies
main.o: main.cpp lexer.h symbol_table.h error_handler.h grammar.h parser.h
lexer.o: lexer.cpp lexer.h symbol_table.h error_handler.h
symbol_table.o: symbol_table.cpp symbol_table.h error_handler.h
error_handler.o: error_handler.cpp error_handler.h
grammar.o: grammar.cpp grammar.h
parser.o: parser.cpp parser.h grammar.h lexer.h symbol_table.h error_handler.h 