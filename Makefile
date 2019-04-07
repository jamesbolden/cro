CXX=g++
BIN=bin
SRC=src
INCLUDE=include
CXXFLAGS=-O3 -std=gnu++1z -I$(INCLUDE)
EXE=$(BIN)/mp
OBJS=$(BIN)/lexer.cc.o $(BIN)/main.cc.o

all: $(EXE) clean

$(EXE): $(OBJS)
	$(CXX) $^ -o $@

$(BIN)/%.cc.o: $(SRC)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: $(OBJS)
	rm -Rf $^
