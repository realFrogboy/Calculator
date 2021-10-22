all: assembler.cpp assem.cpp disassembler.cpp disas.cpp calculator1.cpp calculator.cpp stack.cpp

ass: assembler.cpp assem.cpp
	gcc -g -O0 -o ass assembler.cpp assem.cpp
dis: disassembler.cpp disas.cpp
	gcc -g -O0 -o disass disassembler.cpp disas.cpp -lstdc++
calc: calculator1.cpp calculator.cpp stack.cpp
	gcc -g -O0 -o calc calculator1.cpp calculator.cpp stack.cpp