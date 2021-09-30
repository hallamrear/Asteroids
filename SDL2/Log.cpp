#include "PCH.h"
#include "Log.h"
#include "TextElement.h"

Log* Log::mInstance = nullptr;

Log::Log()
{
	mMessages = std::vector<std::pair<LogLevel, std::string>>();
	mTextElements = std::vector<TextElement*>();
	mMessageQueue = std::deque<std::pair<LogLevel, std::string>>();

	for (size_t i = 0; i < Settings::Get()->GetMaxLogMessages(); i++)
	{
		mTextElements.push_back(new TextElement(Vector2f(), ""));
		mMessageQueue.emplace_back(LogLevel::LOG_MESSAGE, "");
	}
}

Log::~Log()
{
	mMessages.clear();

	delete mInstance;
	mInstance = nullptr;
}

void Log::LogMessage(LogLevel indicator, const char* str)
{
	LogMessage(indicator, std::string(str));
}

void Log::LogMessage(LogLevel indicator, std::string str)
{
	Get()->LogMessage_Impl(indicator, str);
}

void Log::LogMessage_Impl(LogLevel indicator, std::string str)
{
	mMessages.emplace_back(indicator, str);
	mMessageQueue.push_back(std::make_pair(indicator, str));

	if (mMessageQueue.size() > Settings::Get()->GetMaxLogMessages())
		mMessageQueue.pop_front();

	printf("");
}

Log* Log::Get()
{
	if (!mInstance)
		mInstance = new Log();

	return mInstance;
}

void Log::Render_Impl(SDL_Renderer& renderer)
{
	SDL_Rect rect
	{
		50,
		50,
		(int)(Settings::Get()->GetWindowDimensions().X) / 6,
		(int)(Settings::Get()->GetWindowDimensions().Y) - 100,
	};

	SDL_Color colour {64, 64, 64, (256 / 4) * 3};
	SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 64);
	SDL_RenderFillRect(&renderer, &rect);
	SDL_SetRenderDrawColor(&renderer, 255, 255, 0, 128);
	SDL_RenderDrawRect(&renderer, &rect);

	float horizontal = 50.0f + ((Settings::Get()->GetWindowDimensions().X) / 12.0f);
	float vertical = 55;
	for (int i = Settings::Get()->GetMaxLogMessages() - 1; i >= 0; i--)
	{
		vertical += 5;
		vertical += mTextElements[i]->GetTextureSize().Y;
		mTextElements[i]->SetPosition(Vector2f(horizontal, vertical));
		mTextElements[i]->Render();
	}
}

void Log::Update_Impl(double DeltaTime)
{
	//todo : reorder how it updates the list

	for (auto& itr : mTextElements)
		itr->SetString("");

	int loopCount = Settings::Get()->GetMaxLogMessages() - 1;

	for (int i = loopCount; i >= 0; i--)
	{
		mTextElements[i]->SetString(mMessageQueue[i].second);

		switch (mMessageQueue[i].first)
		{
		case LogLevel::LOG_MESSAGE:
			mTextElements[i]->SetColour(Colour(0, 0, 255, 255));
			break;

		case LogLevel::LOG_ERROR:
			mTextElements[i]->SetColour(Colour(255, 0, 0, 255));
			break;

		case LogLevel::LOG_WARNING:
			mTextElements[i]->SetColour(Colour(255, 255, 0, 255));
			break;

		default:
			break;
		}
	}	

	for (auto& itr : mTextElements)
		itr->Update(DeltaTime);
}

void Log::Render(SDL_Renderer& renderer)
{
	Get()->Render_Impl(renderer);
}

void Log::Update(double DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}