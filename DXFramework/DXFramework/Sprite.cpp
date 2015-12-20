#include "stdafx.h"
#include "Sprite.h"

#include <algorithm>


Sprite::Sprite(const Texture &texture) :
	texture(texture),
	position(0, 0),
	size(texture.getSize().x, texture.getSize().y),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	rotation(0),
	positionAnchor(0, 0),
	rotationAnchor(0.5f, 0.5f),
	sourceRect(0, 0, 1, 1)
{
}

Sprite::~Sprite()
{
}

void Sprite::setPosition(float x, float y)
{
	position = { x, y };
}

void Sprite::setSize(float width, float height)
{
	size = { width, height };
}

void Sprite::setRotation(float radians)
{
	rotation = radians;
}

void Sprite::setPositionAnchor(float x, float y)
{
	positionAnchor = { x, y };
}

void Sprite::setRotationAnchor(float x, float y)
{
	positionAnchor = { x, y };
}

void Sprite::setColor(float r, float g, float b, float a)
{
	color =
	{
		std::max(std::min(r, 1.0f), 0.0f),
		std::max(std::min(g, 1.0f), 0.0f),
		std::max(std::min(b, 1.0f), 0.0f),
		std::max(std::min(a, 1.0f), 0.0f)
	};
}

void Sprite::setTexture(const Texture &texture)
{
	this->texture = texture;
}

void Sprite::setSourceRect(float x, float y, float width, float height)
{
	sourceRect = { x, y, width, height };
}

void Sprite::move(float x, float y)
{
	position = { position.x + x, position.y + y };
}

const XMFLOAT2& Sprite::getPosition() const
{
	return position;
}

const XMFLOAT2& Sprite::getSize() const
{
	return size;
}

float Sprite::getRotation() const
{
	return rotation;
}

const XMFLOAT2& Sprite::getPositionAnchor() const
{
	return positionAnchor;
}

const XMFLOAT2& Sprite::getRotationAnchor() const
{
	return rotationAnchor;
}

const XMFLOAT4& Sprite::getColor() const
{
	return color;
}

const XMFLOAT4& Sprite::getSourceRect() const
{
	return sourceRect;
}

const Texture& Sprite::getTexture() const
{
	return texture;
}
