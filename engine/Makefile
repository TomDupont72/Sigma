CXX = g++
CXXFLAGS = -Wall -std=c++20 -Iinclude

SRC = \
	src/domain/tree.cpp \
	src/domain/function.cpp \
	src/domain/token.cpp \
	src/apps/cli/main.cpp \

OBJ = $(SRC:.cpp=.o)

app: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o app

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q src\domain\*.o src\apps\cli\*.o app.exe