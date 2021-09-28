#include "PCH.h"
#include "TextElement.h"
#include "Log.h"
#include "Game.h"

//todo: add font to constuctor
//todo: create font cache

TextElement::TextElement(Vector2f position, std::string string, float rotation, float size, Colour colour)
{
	mTextTexture = nullptr;
	mIsShowing = true;
	mData = string;
	mColour = colour;
	mFontSize = size;
	mRotation = rotation;
	mPosition = position;
	mTextWidth = NULL;
	mTextHeight = NULL;
	mIsDirty = true;
}

TextElement::~TextElement()
{
	mIsDirty = false;
}

void TextElement::DestroyTexture()
{
	SDL_DestroyTexture(mTextTexture);
}

void TextElement::CreateTexture()
{
	//This should not be getting recreated each frame
	TTF_Font* font;
	font = TTF_OpenFont("hyperspace.ttf", (int)mFontSize);
	if (!font) 
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO LOAD FONT");
		Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
	}

	SDL_Renderer& renderer = const_cast<SDL_Renderer&>(*Game::Renderer);
	SDL_Surface* textSurface = nullptr;
	//// Set color to black
	SDL_Color color = { mColour.R, mColour.G, mColour.B, mColour.A };

	textSurface = TTF_RenderText_Solid(font, mData.c_str(), color);

	if(!textSurface)
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO RENDER TEXT");
		Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		return;
	}

	mTextTexture = SDL_CreateTextureFromSurface(&renderer, textSurface);
	SDL_QueryTexture(mTextTexture, NULL, NULL, &mTextWidth, &mTextHeight);
	
	SDL_FreeSurface(textSurface);
	textSurface = nullptr;
	
	TTF_CloseFont(font);
	font = nullptr;
}

void TextElement::Update(double deltaTime)
{
	if(mIsDirty)
	{
		DestroyTexture();
		CreateTexture();

		if (mTextTexture)
		{
			mIsDirty = false;
		}
	}
}

void TextElement::Render()
{
	if(mIsShowing)
	{
		SDL_Rect destRect{};
		destRect.w = mTextWidth;
		destRect.h = mTextHeight;
		destRect.x = static_cast<int>(mPosition.X) - (destRect.w / 2);
		destRect.y = static_cast<int>(mPosition.Y) - (destRect.h / 2);
		SDL_RenderCopyEx(Game::Renderer, mTextTexture, NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);
	}
}

void TextElement::SetShowing(bool state)
{
	mIsShowing = state;
}

void TextElement::SetString(std::string str)
{
	mData = str;
	mIsDirty = true;
}

void TextElement::SetColour(Colour colour)
{
	mColour = colour;
}

void TextElement::SetString(const char* str)
{
	SetString(std::string(str));
}

void TextElement::SetPosition(Vector2f screenPosition)
{
	mPosition = screenPosition;
}