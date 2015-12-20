#include "stdafx.h"
#include "Sprite.h"
#include <algorithm>

Sprite::Sprite(const Texture2D& texture) :
	m_Texture(texture),
	m_Position(0, 0),
	m_Size(static_cast<float>(texture.getDesc().Width), static_cast<float>(texture.getDesc().Height)),
	m_Color(1.0f, 1.0f, 1.0f, 1.0f),
	m_Rotation(0),
	m_PositionAnchor(0, 0),
	m_RotationAnchor(0.5f, 0.5f),
	m_SourceRect(0, 0, 1, 1)
{
}

void Sprite::SetPosition(float x, float y)
{
	m_Position = { x, y };
}

void Sprite::SetSize(float width, float height)
{
	m_Size = { width, height };
}

void Sprite::SetRotation(float radians)
{
	m_Rotation = radians;
}

void Sprite::SetPositionAnchor(float x, float y)
{
	m_PositionAnchor = { x, y };
}

void Sprite::SetRotationAnchor(float x, float y)
{
	m_PositionAnchor = { x, y };
}

void Sprite::SetColor(float r, float g, float b, float a)
{
	m_Color =
	{
		std::max(std::min(r, 1.0f), 0.0f),
		std::max(std::min(g, 1.0f), 0.0f),
		std::max(std::min(b, 1.0f), 0.0f),
		std::max(std::min(a, 1.0f), 0.0f)
	};
}

void Sprite::SetTexture(const Texture2D& texture)
{
	m_Texture = texture;
}

void Sprite::SetSourceRect(float x, float y, float width, float height)
{
	m_SourceRect = { x, y, width, height };
}

void Sprite::Move(float x, float y)
{
	m_Position = { m_Position.x + x, m_Position.y + y };
}

const XMFLOAT2& Sprite::GetPosition() const
{
	return m_Position;
}

const XMFLOAT2& Sprite::GetSize() const
{
	return m_Size;
}

float Sprite::GetRotation() const
{
	return m_Rotation;
}

const XMFLOAT2& Sprite::GetPositionAnchor() const
{
	return m_PositionAnchor;
}

const XMFLOAT2& Sprite::GetRotationAnchor() const
{
	return m_RotationAnchor;
}

const XMFLOAT4& Sprite::GetColor() const
{
	return m_Color;
}

const XMFLOAT4& Sprite::GetSourceRect() const
{
	return m_SourceRect;
}

const Texture2D& Sprite::GetTexture() const
{
	return m_Texture;
}
