#pragma once
#include "Entity.h"
#include "Colour.h"

class TextElement
	: protected Entity
{
private:
	bool		mIsDirty;
	std::string mData;
	Colour		mColour;

	void DestroyTexture();
	void CreateTexture();

public:
	TextElement(
		SDL_Renderer& renderer, 
		Vector2f position,
		std::string string = "", 
		float rotation = 0.0f,
		float size = 14.0f, 
		Colour colour = Colour());
	~TextElement() override;

	void Update(double deltaTime) override;
	void Render() override;

	void SetShowing(bool state);
	void SetColour(Colour colour);
	void SetString(std::string str);
	void SetString(const char* str);
};

