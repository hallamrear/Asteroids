#include "PCH.h"
#include "TextElement.h"
#include "Log.h"

//todo: add font to constuctor
//todo: create font cache

TextElement::TextElement(SDL_Renderer& renderer, Vector2f position, std::string string, float rotation, float size, Colour colour)
	: Entity(renderer, "", position, rotation, 0.0f, 0.0f, 0.0f)
{
	mData = string;
	mColour = colour;
	mFontSize = size;

	SetPhysicsEnabled(false);
	SetDragEnabled(false);

	mIsDirty = true;
}

TextElement::~TextElement()
{
	mIsDirty = false;
}

void TextElement::DestroyTexture()
{
	if(mTexture)
	{
		SDL_DestroyTexture(mTexture);
	}	
}

void TextElement::CreateTexture()
{
	TTF_Font* font;
	font = TTF_OpenFont("hyperspace.ttf", (int)mFontSize);
	if (!font) 
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO LOAD FONT");
		Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
	}

	SDL_Renderer& renderer = const_cast<SDL_Renderer&>(GetRendererReference());
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

	mTexture = SDL_CreateTextureFromSurface(&renderer, textSurface);
	SDL_QueryTexture(mTexture, NULL, NULL, &mTextureSizeX, &mTextureSizeY);
	
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

		if (mTexture)
		{
			mIsDirty = false;
		}
	}
}

void TextElement::Render()
{
	if(GetIsAlive())
	{
		SDL_Rect destRect{};
		destRect.w = mTextureSizeX;
		destRect.h = mTextureSizeY;
		destRect.x = static_cast<int>(mPosition.X) - (destRect.w / 2);
		destRect.y = static_cast<int>(mPosition.Y) - (destRect.h / 2);
		SDL_RenderCopyEx(&mRenderer, mTexture, NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);
	}
}

void TextElement::SetShowing(bool state)
{
	SetAlive(state);
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