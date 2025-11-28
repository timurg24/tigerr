#pragma once
/*
	TiGErr C++ Error logging library
	Version 1.0.1B
	Copyright 2025 Timur Gabdrakhmanov. MIT License.
*/
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#ifdef NDEBUG
#define TIGERR_RELEASE_MODE
#endif

	inline void LogToFile(std::string filePath, std::string contents) {
		std::filesystem::path fsPath(filePath);
		if (std::filesystem::exists(fsPath)) {
			std::ofstream file(filePath, std::ios::app);
			if (!file.is_open()) {
				std::cout << "LogError Cannot Open Log File! \"" << filePath << "\". Contact Developer with Code 201!\n";
			}
			file << contents;
		}
		else {
			std::cout << "LogError Cannot Find Log File! \"" << filePath << "\". Contact Developer with Code 200!\n";
		}
	}

	enum Severity {
		DEBUG, // Used for debugging messages, disabled when Release build is selected in MSVC or the release macro is enabled manually
		NOTICE, // Used for simple logging
		ALERT, // An error that is not good to see but not mission critical
		FAILURE, // A critical failure
	};

	enum LogOutput {
		CONSOLE    = 1u << 0, // Prints out the the standard application console
		FILE       = 1u << 1, // Logs to file
	};



	/// <summary>
	/// Advanced error logging function
	/// </summary>
	/// <param name="errorLabel"> - Error title or error code </param>
	/// <param name="errorDetails"> - More in-depth information about error </param>
	/// <param name="errorSeverity"> - Criticality of the error </param>
	/// <param name="asciiColor"> - Toggle for color usage when printing to console </param>
	/// <param name="outout"> - Enum controlling the output of the error </param>
	/// <param name="logPath"> - Path for Log file, only used when output is set to FILE</param>
	inline void LogError(std::string errorLabel, std::string errorDetails, Severity errorSeverity, bool asciiColor, LogOutput output, std::string logPath) {
		
		std::string message = "";
		std::string prefix = "";
		std::string colorCode = "";
		std::string resetCode = "\033[0m";

		switch (errorSeverity) {
			case DEBUG: prefix = "DEBUG: "; colorCode = "\033[1;37m"; break;
			case NOTICE: prefix = "NOTICE: "; colorCode = resetCode; break;
			case ALERT: prefix = "ALERT: ";  colorCode = "\033[33m"; break;
			case FAILURE: prefix = "FAILURE: "; colorCode = "\033[1;31m"; break;
			default: std::cout << "LogError Severity Error! Contact Developer with Code 100!\n"; break;
		}

		message = (prefix + errorLabel + ": " + errorDetails + "\n");
		
		if (output & FILE) {
			if (errorSeverity == DEBUG) {
				#ifndef TIGERR_RELEASE_MODE
					LogToFile(logPath, message); // Only logs to file if debug mode is on and the DEBUG log severity is selected
				#endif
			}
			else LogToFile(logPath, message);
			
		}

		if (asciiColor)
			message = colorCode + message + resetCode;

		if (output & CONSOLE) {
			if (errorSeverity == DEBUG) {
				#ifndef TIGERR_RELEASE_MODE
				std::cout << message; // Only prints this message when RELEASE MODE is not defined (aka Debug Mode)
				#endif
			} else std::cout << message;
			
		}

	}
/*
	Error Code Lookup:
	Code 100 - Invalid Error Severity, check "errorSeverity" parameter and make sure its either NOTICE, ALERT, or FAILURE
	Code 200 - Cannot find specified Log file, check "logPath"
	Code 201 - For some reason couldn't open your desired "logPath", I have no idea how to fix this
*/
