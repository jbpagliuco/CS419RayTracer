#include <stdlib.h>
#include <crtdbg.h>

#include "Log.h"
#include <Engine.h>
#include <P4Mem.h>

void DoStuff();


int main(char ** argv, int argc)
{
	// Change console font size
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = 28;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	DoStuff();

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	RE_LOG(MEMORY, END, "Dumping memory leaks...");

	P4::ReportMemory();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << std::endl;

	system("pause");

	return 0;
}


void DoStuff()
{
	RE::Engine engine;

	// Initialize engine and all subsystems.
	engine.Initialize("engine.config");

	// Run ray tracer.
	engine.Run();

	system("pause");

	// Unload engine and all subsystems.
	engine.Unload();

	RE_LOG_EMPTY();
}