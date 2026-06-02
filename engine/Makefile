CXX = g++
CXXFLAGS = -Wall -std=c++20 -Iinclude
DEPFLAGS = -MMD -MP

SRC = \
	src/domain/tree.cpp \
	src/domain/function.cpp \
	src/domain/token.cpp \
	src/domain/parser.cpp \
	src/algorithms/lexer.cpp \
	src/algorithms/display/display.cpp \
	src/algorithms/display/parts.cpp \
	src/algorithms/simplify/simplify.cpp \
	src/algorithms/simplify/operations.cpp \
	src/algorithms/simplify/rewrite_rules.cpp \
	src/algorithms/simplify/sum_rules.cpp \
	src/algorithms/derive/derive.cpp \
	src/algorithms/derive/rules.cpp \
	src/algorithms/utils.cpp \
	src/apps/cli/main.cpp

OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

ifeq ($(OS),Windows_NT)
all: app app.exe

app.exe: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o app.exe

app: app.exe
	powershell -NoProfile -Command "Copy-Item -Force -LiteralPath app.exe -Destination app"
else
all: app

app: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o app
endif

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

.PHONY: all clean rebuild
rebuild: clean all

clean:
ifeq ($(OS),Windows_NT)
	powershell -NoProfile -Command "$$paths = @($(foreach f,$(OBJ) $(DEP) app app.exe,'$(subst /,\,$f)';)); Remove-Item -Force -ErrorAction SilentlyContinue -LiteralPath $$paths; exit 0"
else
	rm -f $(OBJ) $(DEP) app app.exe
endif

-include $(DEP)
