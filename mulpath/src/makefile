#-----------------------------------------------
# WARNING!
#-----------------------------------------------
# This makefile should only be used in Windows
# and Visual Studio 2012. In addition, the
# "make" must run in "Developer Command Prompt
# for VS2012".
#-----------------------------------------------

TARGET = main.exe
HEADERS = $(wildcard include/*.h)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.obj)

all: $(TARGET)

clean:
	del *.obj $(TARGET)
	del gen.exe

run: all ../testcase/out1.txt ../testcase/out2.txt ../testcase/out3.txt ../testcase/out4.txt ../testcase/out5.txt ../testcase/out6.txt ../testcase/out7.txt ../testcase/out8.txt ../testcase/out9.txt ../testcase/out10.txt

$(TARGET): $(OBJECTS)
	cl /Ox /O2 /Ot /favor:ATOM /EHsc $(OBJECTS) /Femain /MT lib/gurobi_c++mt2012.lib lib/gurobi65.lib /F268435456
	
# /Ox /O2 /Ot /favor:ATOM
%.obj: %.cpp $(HEADERS)
	cl /Ox /O2 /Ot /favor:ATOM /EHsc $< /c

gen.exe: ../testcase/gen.cpp
	g++ -O2 ../testcase/gen.cpp -o gen.exe

# easy case
../testcase/in1.txt: gen.exe
	gen.exe 8 8 20 3 3 4 0 > ../testcase/in1.txt

# very large set of terminals
../testcase/in2.txt: gen.exe
	gen.exe 9 9 25 3 3 6 0 > ../testcase/in2.txt

# many obstacles
../testcase/in3.txt: gen.exe
	gen.exe 10 10 30 3 3 10 0 > ../testcase/in3.txt

# large set of terminals and large terminals
../testcase/in4.txt: gen.exe
	gen.exe 12 12 45 3 3 9 0 > ../testcase/in4.txt

# only one terminal set with many obstacles
../testcase/in5.txt: gen.exe
	gen.exe 15 15 70 3 3 15 0 > ../testcase/in5.txt

# large map
../testcase/in6.txt: gen.exe
	gen.exe 18 18 100 3 3 24 0 > ../testcase/in6.txt

# difficult case
../testcase/in7.txt: gen.exe
	gen.exe 8 8 20 2 3 4 0 > ../testcase/in7.txt

# rectangle case
../testcase/in8.txt: gen.exe
	gen.exe 10 10 30 2 3 10 0 > ../testcase/in8.txt

# rectangle case with many terminal sets
../testcase/in9.txt: gen.exe
	gen.exe 15 15 75 2 3 0 0 > ../testcase/in9.txt

# mixed many terminal sets of 2 and 3 size
../testcase/in10.txt: gen.exe
	gen.exe 18 18 100 2 3 24 0 > ../testcase/in10.txt

../testcase/out1.txt: main.exe ../testcase/in1.txt
	main.exe < ../testcase/in1.txt > ../testcase/out1.txt

../testcase/out2.txt: main.exe ../testcase/in2.txt
	main.exe < ../testcase/in2.txt > ../testcase/out2.txt

../testcase/out3.txt: main.exe ../testcase/in3.txt
	main.exe < ../testcase/in3.txt > ../testcase/out3.txt

../testcase/out4.txt: main.exe ../testcase/in4.txt
	main.exe < ../testcase/in4.txt > ../testcase/out4.txt

../testcase/out5.txt: main.exe ../testcase/in5.txt
	main.exe < ../testcase/in5.txt > ../testcase/out5.txt

../testcase/out6.txt: main.exe ../testcase/in6.txt
	main.exe < ../testcase/in6.txt > ../testcase/out6.txt

../testcase/out7.txt: main.exe ../testcase/in7.txt
	main.exe < ../testcase/in7.txt > ../testcase/out7.txt

../testcase/out8.txt: main.exe ../testcase/in8.txt
	main.exe < ../testcase/in8.txt > ../testcase/out8.txt

../testcase/out9.txt: main.exe ../testcase/in9.txt
	main.exe < ../testcase/in9.txt > ../testcase/out9.txt

../testcase/out10.txt: main.exe ../testcase/in10.txt
	main.exe < ../testcase/in10.txt > ../testcase/out10.txt
