#pragma once

enum class LogLevel
{
	LOG_ERROR = 0,
	LOG_MESSAGE,
};

class Log
{
private:
	static Log* mInstance;

	Log();
	~Log();
	static Log* Get();
	std::vector<std::pair<LogLevel, std::string>> messages;
	void LogMessage_Internal(LogLevel indicator, std::string str);

public:
	static void LogMessage(LogLevel indicator, std::string str);
	static void LogMessage(LogLevel indicator, const char* str);
};

