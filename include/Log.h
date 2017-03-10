#pragma once

#include <string>
#include <sstream>

#define RE_LOG(sys, cat, s) { std::ostringstream ss; ss << s; RE::Log(#sys, #cat, ss.str()); }
#define RE_LOGERROR(sys, cat, s) { std::ostringstream ss; ss << s; RE::LogError(#sys, #cat, ss.str()); }
#define RE_LOGDEBUG(s) { std::ostringstream ss; ss << s; RE::LogDebug(ss.str(), __FILE__, __LINE__); }
#define RE_LOG_EMPTY() std::cout << std::endl;
#define RE_LOG_SAMELINE(sys, cat, s) { std::ostringstream ss; ss << s; RE::LogSameLine(#sys, #cat, ss.str()); }
#define RE_LOG_NEWLINE() { std::cout << std::endl; }

namespace RE
{
	// Logs an info message to stdout.
	// @param system: The system that is logging this message.
	// @param cat: The category for this message.
	// @param msg: The message to output.
	template <typename T>
	void Log(const std::string& system, const std::string& cat, const T& msg);
	
	// Logs an error message to stdout.
	// @param system: The system that is logging this message.
	// @param cat: The category for this message.
	// @param msg: The message to output.
	template <typename T>
	void LogError(const std::string& system, const std::string& cat, const T& msg);

	// Logs a message to stdout, overlaps the last line.
	// @param system: The system that is logging this message.
	// @param cat: The category for this message.
	// @param msg: The message to output.
	template <typename T>
	void LogSameLine(const std::string& system, const std::string& cat, const T& msg);

	// Logs a debug message to stdout.
	// @param msg: The message to output.
	// @param file: The file that this statement is in.
	// @param line: The line that this statement is on.
	template <typename T>
	void LogDebug(const T& msg, const std::string& file, int line);
}



#include <Windows.h>
#include <iostream>

namespace RE
{
	template <typename T>
	void Log(const std::string& system, const std::string& cat, const T& msg)
	{
		WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		if (cat == "INIT")
			color = FOREGROUND_GREEN;
		else if (cat == "RUNTIME")
			color = FOREGROUND_GREEN | FOREGROUND_BLUE;
		else if (cat == "END")
			color = FOREGROUND_RED | FOREGROUND_BLUE;
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | FOREGROUND_INTENSITY);
		std::cout << "[" << system << "] (" << cat << "): " << msg << std::endl;
	}

	template <typename T>
	void LogError(const std::string& system, const std::string& cat, const T& msg)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "[" << system << "] (" << cat << ") ERROR: " << msg << std::endl;
	}

	template <typename T>
	void LogSameLine(const std::string& system, const std::string& cat, const T& msg)
	{
		WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		if (cat == "INIT")
			color = FOREGROUND_GREEN;
		else if (cat == "RUNTIME")
			color = FOREGROUND_GREEN | FOREGROUND_BLUE;
		else if (cat == "END")
			color = FOREGROUND_RED | FOREGROUND_BLUE;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | FOREGROUND_INTENSITY);
		std::cout << "\r[" << system << "] (" << cat << "): " << msg << "    ";
	}

	template <typename T>
	void LogDebug(const T& msg, const std::string& file, int line)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "[DBUG]: " << std::endl << 
			"    (File: " << file << ") " << std::endl <<
			"    (Line: " << line << ") " << std::endl <<
			"    " << msg << std::endl;
	}
}