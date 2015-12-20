#pragma once

#include "Texture.h"

class Sprite
{
public:
	Sprite(const Texture &texture);
	virtual ~Sprite();

	void setPosition(float x, float y);
	void setSize(float width, float height);
	void setRotation(float radians);
	void setPositionAnchor(float x, float y);
	void setRotationAnchor(float x, float y);
	void setColor(float r, float g, float b, float a);
	void setTexture(const Texture &texture);
	void setSourceRect(float x, float y, float width, float height);
	void move(float x, float y);

	const XMFLOAT2& getPosition() const;
	const XMFLOAT2& getSize() const;
	float getRotation() const;
	const XMFLOAT2& getPositionAnchor() const;
	const XMFLOAT2& getRotationAnchor() const;
	const XMFLOAT4& getColor() const;
	const XMFLOAT4& getSourceRect() const;

	const Texture& getTexture() const;
private:
	Texture texture;
	XMFLOAT2 position;
	XMFLOAT2 size;
	XMFLOAT4 color;
	float rotation;
	XMFLOAT2 positionAnchor;
	XMFLOAT2 rotationAnchor;
	XMFLOAT4 sourceRect;
};

