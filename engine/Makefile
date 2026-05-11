CXX = g++
CXXFLAGS = -Wall -std=c++20 -Iinclude
DEPFLAGS = -MMD -MP

SRC = \
	src/domain/tree.cpp \
	src/domain/function.cpp \
	src/domain/token.cpp \
	src/algorithms/lexer.cpp \
	src/apps/cli/main.cpp

OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

app: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o app

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-del /Q $(subst /,\,$(OBJ)) $(subst /,\,$(DEP)) app.exe 2>NUL

-include $(DEP)
