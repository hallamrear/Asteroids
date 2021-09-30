#pragma once
#include "Entity.h"
#include "Colour.h"

class TextElement
{
private:
	SDL_Texture*	mTextTexture;
	Vector2f		mPosition;
	float			mRotation;
	bool			mIsDirty;
	bool			mIsShowing;
	std::string		mData;
	Colour			mColour;
	float			mFontSize;
	int				mTextWidth;
	int				mTextHeight;

	void DestroyTexture();
	void CreateTexture();

public:
	TextElement(
		Vector2f position,
		std::string string = "", 
		float rotation = 0.0f,
		float size = 14.0f, 
		Colour colour = Colour());
	~TextElement();

	void		Update(double deltaTime);
	void		Render();

	void		SetShowing(bool state);
	void		SetColour(Colour colour);
	void		SetString(std::string str);
	void		SetString(const char* str);
	void		SetPosition(Vector2f screenPosition);
	Vector2f	GetTextureSize();
};

