# include <stdio.h>
# include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Mem {
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	void Initialise(){
		for (u32 i = 0; i<MAX_MEM; i++){
			Data[i] = 0;
		}
	}

	// read byte function
	Byte operator[](u32 Address) const {
		return Data[Address];
	}

	// write byte function
	Byte& operator[](u32 Address) {
		return Data[Address];
	}
};


struct CPU {

	Word PC;    // progra counter
	Word SP;    // stack pointer

	// registers
	Byte A;     // accumulator
	Byte X, Y;  // index registers X, Y

	// processor state
	// but fields
	Byte C : 1; // carry
	Byte Z : 1; // zero
	Byte I : 1; // interrupt disable
	Byte D : 1; // Decimal mode
	Byte B : 1; // break
	Byte V : 1; // overflow
	Byte N : 1; // negative

	void Reset(Mem& memory)
	{
		// sample reset where pointers are set and flags are cleared. 
		// The exact values to set pointers and registers may 
		// vary but this should be sufficient for emulation.
		PC = 0xFFFC;    // first memroy address read on start/reset
		SP = 0x0100;
		C = D = 0;
		A = X = Y = 0;
		memory.Initialise();
	}

	//opcodes
	static constexpr Byte LDA_IMM = 0xA9;


	// Cycles: number of ticks or clock cycles that instr executes for
	void Execute(u32 Cycles, Mem& memory){
		while (Cycles > 0) {
			Byte Instr = Fetch(Cycles, memory);
			switch(Instr){
				case LDA_IMM:
				{
					Byte Value = Fetch(Cycles, memory);
					A = Value;
					Z = (A == 0);
					N = (A & 0b10000000) > 0;

				} break;
				default:
				{
					printf("Instruction not handled %d\n", Instr);
				}break;
			}
		}
	}

	Byte Fetch(u32& Cycles, Mem& memory) {
		Byte Data = memory[PC];
		PC++;
		Cycles--;
		return Data;
	}
};

int main(){
	Mem mem;
	CPU cpu;
	cpu.Reset(mem);    // need to reset cpu on start

	//inline little program
	mem[0xFFFC] = CPU::LDA_IMM;
	mem[0xFFFd] = 0x50;
	//inline little program
	cpu.Execute(2, mem);

	return 0;
}


