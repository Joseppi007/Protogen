// Protogen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <conio.h>
#include <time.h>

unsigned long paramsAsAddress(std::vector<char> params)
{
	unsigned long address = 0;
	for (unsigned long i = params.size(); i > 0; i--)
	{
		address *= 256;
		address += (unsigned char)(params[i-1]);
	}
	return address;
}

void protogen(char *filePath)
{
	std::vector<unsigned long> depthStack = std::vector<unsigned long>();
	std::string characterInputQue = std::string();
	unsigned char regesterA = 0, regesterB = 0, regesterC = 0, regesterE = 0;
	unsigned long width = 4;
	std::ifstream file(filePath, std::ios::binary);
	std::vector<char> srcCode ((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));
	file.close();

	width = 1;
	for (int i = srcCode[0]; i > 0; i--)
	{
		width *= 2;
	}

	unsigned long currentLinePosition = width;
	depthStack.push_back(width);
	while (depthStack.size() > 0)
	{
		currentLinePosition = depthStack.back();
		depthStack.pop_back();
		bool runningCodeBlock = true;
		while (runningCodeBlock && currentLinePosition < srcCode.size())
		{
			std::vector<char> params = std::vector<char>();
			for (unsigned long i = currentLinePosition+1; i%width != 0; i++)
			{
				params.push_back(srcCode[i]);
			}
			signed int shouldBe; // Where do I put this?
			unsigned short int in;
			switch (srcCode[currentLinePosition])
			{
			case 'A':
				regesterA = params[0];
				break;
			case 'B':
				regesterB = params[0];
				break;
			case 'a':
				regesterA = srcCode[paramsAsAddress(params)];
				break;
			case 'b':
				regesterB = srcCode[paramsAsAddress(params)];
				break;
			case 'c':
				srcCode[paramsAsAddress(params)] = regesterC;
				break;
			case 'e':
				srcCode[paramsAsAddress(params)] = regesterE;
				break;
			case '+':
				regesterC = regesterA + regesterB;
				shouldBe = ((signed int)regesterA) + ((signed int)regesterB);
				if (regesterC == shouldBe)
				{
					regesterE = 0;
				}
				else
				{
					regesterE = 1;
				}
				break;
			case '-':
				regesterC = regesterA - regesterB;
				shouldBe = ((signed int)regesterA) - ((signed int)regesterB);
				if (regesterC == shouldBe)
				{
					regesterE = 0;
				}
				else
				{
					regesterE = 255;
				}
				break;
			case '&':
				regesterC = regesterA & regesterB;
				regesterE = 0;
				break;
			case '|':
				regesterC = regesterA | regesterB;
				regesterE = 0;
				break;
			case '^':
				regesterC = regesterA ^ regesterB;
				regesterE = 0;
				break;
			case 'j':
				currentLinePosition = paramsAsAddress(params) - width;
				break;
			case '>':
				if (regesterA > regesterB)
				{
					currentLinePosition = paramsAsAddress(params) - width;
				}
				break;
			case '=':
				if (regesterA == regesterB)
				{
					currentLinePosition = paramsAsAddress(params) - width;
				}
				break;
			case '<':
				if (regesterA < regesterB)
				{
					currentLinePosition = paramsAsAddress(params) - width;
				}
				break;
			case 'r':
				depthStack.push_back(currentLinePosition+width);
				currentLinePosition = paramsAsAddress(params) - width;
				break;
			case 0:
			case '0':
				runningCodeBlock = false;
				break;
			case ',':
				while (characterInputQue.size() == 0)
				{
					std::string in;
					std::cin >> in;
					characterInputQue += in;
				}
				if (characterInputQue.size() == 1)
				{
					regesterE = 1;
				}
				else
				{
					regesterE = 0;
				}
				regesterC = characterInputQue[0];
				characterInputQue = characterInputQue.substr(1);
				break;
			case '.':
				std::cout << regesterA;
				break;
			case ';':
				characterInputQue = "";
				std::cin >> in;
				regesterC = in % 256;
				regesterE = (in / 256) % 256;
				break;
			case ':':
				std::cout << (unsigned int)regesterA;
				break;
			case '?':
				regesterC = regesterA + rand() % (regesterB - regesterA + 1);
				regesterE = 0;
				break;
			}
			currentLinePosition += width;
		}
	}
	
	std::cout << "\n\nPress [ENTER] to end program. ";
	_getch();
	std::cout << "\n";
}

int main(int argc, char *argv[])
{
	srand((unsigned int)std::time(0));

	if (argc == 1)
	{
		std::cout << "Umm... Think you can put the path to the code in as a command line argument?\nI think Windows will let you open the file with this code and that should work probably.\n\nPress any key. ";
		_getch();
		std::cout << "\nActually, why don't you just give me the path now? (Just use the command line arguments next time, ok?)\nPATH> ";
		std::string thePath;
		std::cin >> thePath;
		std::cout << "Ok, I'll run " << thePath << "now...\n\n";
		protogen((char*)thePath.c_str());
	}
	for (int i = 1; i < argc; i++)
	{
		protogen(argv[i]);
		delete argv[i];
	}
	if (argc > 2)
	{
		std::cout << "\n\nAll programs have been run. Press [ENTER] to stop. ";
		_getch();
	}

	delete argv;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
