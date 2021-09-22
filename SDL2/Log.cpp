#include "PCH.h"
#include "Log.h"

Log* Log::mInstance = nullptr;

void Log::LogMessage(LogLevel indicator, const char* str)
{
	LogMessage(indicator, std::string(str));
}

void Log::LogMessage(LogLevel indicator, std::string str)
{
	Get()->LogMessage_Internal(indicator, str);
}

void Log::LogMessage_Internal(LogLevel indicator, std::string str)
{
	messages.emplace_back(indicator, str);
}

Log::Log()
{
	messages = std::vector<std::pair<LogLevel, std::string>>();
}

Log::~Log()
{
	messages.clear();

	delete mInstance;
	mInstance = nullptr;
}

Log* Log::Get()
{
	if (mInstance)
		return mInstance;
	else
		mInstance = new Log();

	return mInstance;
}