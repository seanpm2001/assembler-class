//need to create the functions run the array notation. 

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctype.h>

using namespace std;
char ASM_FILE_NAME[] = "testFile1.asm";

const int MAX = 150;  //size of simulators memory
const int COL = 7;	 //number of columns for output

			//REGISTERS
const int AXREG = 0;
const int BXREG = 1;
const int CXREG = 2;
const int DXREG = 3;

         //commands
const int HALT = 5;
const int MOVREG = 192;

enum paramType {reg, mem, constant, arrayBx, arrayBxPlus, none};

typedef short int Memory;

class Registers
{
public:
	int AX;
	int BX;
	int CX;
	int DX;
	int instrAddr;
	int flag;
}regis;
Memory memory[MAX] = {0};
int address;

void printMemoryDump(); //prints memeory with integers commands
void convertToMachineCode(ifstream &fin);
void convertToNumber(string line, int &start, int &value);
int whichReg(char regLetter);  //returns the number of the letter registar
void fillMemory( );
void runCode( );
void changeToLowerCase(string &line);
void splitCommand( string &line, string &command )
{
	int space = line.find( ' ' );
	command = line.substr( 0, space );
	line = line.substr(space + 1);
}
void convertToMachineCode( ifstream &fin )
{
	string line;  //full command
	string commArr[3];
	string command; //the asm commmand
	string oper1, oper2;  //the two operands could be empty
	int machineCode = 0;
	getline( fin, line, '\n' );
	changeToLowerCase( line );

	int i = 0;

	while( line.length() > 0 || i < 2)
	{
		splitCommand( line, command );
		commArr[i] = command; 
		if (i==2){ line = "";}
		i++;
	}

	command = commArr[0];
	if(i > 0)
		oper1 = commArr[1];
	if(i > 1)
		oper2 = commArr[2];
	
	if (command[0] == 'h')  //halt
	{
		memory[address] = HALT;
		address++;
	}
	if (command[0] == 'm')  //move into a register
	{
		machineCode = MOVREG;
		machineCode += (whichReg( oper1[0] ) << 3);
		machineCode += 7; 
		memory[address] = machineCode;
		address++;
		 //convertToNumber(commArr[2], 0, memory[address]);
		 memory[address] = stoi(commArr[2]);
		address++;
	}
	cout << endl;
	//else if (command)


}

int main( )
{
	printMemoryDump( );
	fillMemory( );
	printMemoryDump( );
	runCode( );
	printMemoryDump( );
	cout << endl;
	system( "pause" );
	return 0;
}
/************************************************************/
/*fillMemory																*/
/*		changes the code to machine code and places the
commands into the memory. */
void fillMemory( )
{
	address = 0;
	ifstream fin;
	//recommend changeing so you can type in file name
	fin.open( ASM_FILE_NAME );
	if (fin.fail( ))
	{
		cout << "Error, file didn't open\n";
		system( "pause" );
		exit( 1 );
	}

	for (int i = 0; i< MAX && !fin.fail( ); i++)
	{
		convertToMachineCode( fin );
	}
	//cout<<"memory filled";
	
}


/************************************************************/
/*whichReg																	*/
/*		changes the letter of the registar to a number.			*/

int whichReg(char regLetter)
{
	if (regLetter == 'a')
	{
		return AXREG;
	}
	else if (regLetter == 'b')
	{
		return BXREG;
	}
	else if (regLetter == 'c')
	{
		return CXREG;
	}
	else if (regLetter == 'd')
	{
		return DXREG;
	}
	return -1;  //something went wrong if -1 is returned
}

/************************************************************/
/*ConvertToNumber															*/
/*  takes in a line and converts digits to a integer			*/
/*  line - is the string of assembly code to convert			*/
/*  start - is the location where the line is being coverted, 
		it starts at the beginning of number and it passed 
		back at the next location */
/*  value - is the integer value of the digits in the code	*/
void convertToNumber(string line, int &start, int &value)
{
	char number[16];
	bool negative = false;
//	cout<< "in convertToNumber before function 1  start is "<<start<<endl;
	int i = 0;
	if (line[start] == '-')
	{
		start++;
		negative = true;
	}
	while (i<16 && line.size() > start&& isdigit(line[start]))
	{
		number[i] = line[start];
					//	cout<<line[start];
		i++;
		start++;
				//		cout<<i<<start;
	}
	number[i] = '\0';
	value = atoi(number);
	if (negative == true)
	{
		value = -value;
	}
//	cout<< "in convertToNumber after function 1  start is "<<start<<endl;
}

/************************************************************/
/*printMemoryCommands													*/
/*prints memory with letter commands								*/
void printMemoryCommands()
{
	int i= 0;
	int count;  // which column is currently outputting
	int moveColumn = MAX / COL;
	int index=0;
	while ( i< MAX)
	{
		count = 0;
		while (count < COL && index < MAX)
		{
			index = i + count * moveColumn;
			//As each type of instruction is added you will need to be able to deal with type of command
			//printcommand(index)
		}
		cout<<endl;
			
	}
	cout<<"RA "<< regis.AX << "  BX "<< regis.BX << "  CX "<< regis.CX << "  DX "<< regis.DX<<endl;
	cout<<"\tinstruction "<<regis.instrAddr<<"\tflag "<<regis.flag<<"\n\n";
}


/************************************************************/
/*printMemoryDump															*/
/*prints memory by number												*/
/*MAX is the amount of elements in the memory array (Vicki used 100) */
/*COL is the number of columns that are to be displayed (Vicki used 7; was originally called COLUMNS) */
void printMemoryDump()
{
	int numRows=MAX/COL+1;   //number of rows that will print
	int carryOver=MAX%COL;   //number of columns on the bottom row
	int location;   //the current location being called
	for(int row=0;row<numRows;row++)
	{
		location=row;
		for(int column=0;location<MAX&&column<COL;column++)
		{
			if(!(numRows-1==row&&carryOver-1<column))
			{
				cout<<setw(5)<<location<<"."<<setw(5)<<memory[location];
				location+=(numRows-(carryOver-1<column));
			}
		}
		cout<<endl;
		//cout<<setw(3);
	}

	cout<<endl;
	cout<<"AX: "<<regis.AX<<'\t';
	cout<<"BX: "<<regis.BX<<'\t';
	cout<<"CX: "<<regis.CX<<'\t';
	cout<<"DX: "<<regis.DX<<'\t';
	cout<<endl<<endl;
	cout<<"Instruction: "<<regis.instrAddr<<endl;
	cout<<"Flag: "<<regis.flag;
	
	cout<<endl<<endl;
}


void changeToLowerCase(string &line)
{
	
	int index=0;
	while (index < line.size())
	{
		line[index] = tolower(line[index]);
		index++;
	}
//	cout<<"the line in change"<<line;

}

void runCode( )
{
	// needs to be written
	address = 0;
cout << "address is " << address << endl;
	int topBits, midBits, botBits;
	while(memory[address] != 5)  //until HALT
	{
		cout << "contents of memory at " << address << " is " << memory[address] << endl;
		topBits = (memory[address] & 192);
		midBits = (memory[address] & 24) >> 3;
		botBits = memory[address] & 7;
		cout << "topBit :" << topBits << endl << "midBits: " << midBits << endl << "botBits: " << botBits << endl;

		if(topBits ==  MOVREG)
		{
			cout << "The command is to move into a register" << endl;
			if(botBits == 7)
			{
				cout << "The command is to move a constant " << endl;
				switch(midBits){
					case(0):
						cout << "moving into AX" << endl;
						regis.AX = memory[address+1];
						break;
					case(1):
						regis.BX = memory[address+1];
						break;
					case(2):
						regis.CX = memory[address+1];
						break;
					case(3):
						regis.DX = memory[address+1];
						break;
				}
				address++;
			}
		}
		address++;
	}
}
