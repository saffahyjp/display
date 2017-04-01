#———————————————————————
# WARNING!
#-----------------------------------------------
# This makefile should only be used in Windows
# and Visual Studio 2012. In addition, the
# "make" must run in "Developer Command Prompt
# for VS2012".
#-----------------------------------------------

TARGET = main.exe
HEADERS = $(wildcard include/*.h)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.obj)

all: $(TARGET)

clean:
	rm -f build/*.obj $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -Wall -std=c++11 -O3 $(OBJECTS) libgurobi_c++.a -lgurobi65 -o $@
# LINK $(OBJECTS) lib/gurobi_c++mt2012.lib lib/gurobi65.lib /PROFILE /OUT:$@ /STACK:268435456 /DEBUG
# cl $(OBJECTS) /Fe$@ /MT lib/gurobi_c++mt2012.lib lib/gurobi65.lib /F268435456
	
# debug: cl /Zi /W4 /WX /DENABLE_ASSERT /EHsc
# release: cl /Ox /O2 /Ot /W4 /WX /EHsc
build/%.obj: src/%.cpp $(HEADERS)
	g++ -Wall -std=c++11 -O3 $< -c -o $@
