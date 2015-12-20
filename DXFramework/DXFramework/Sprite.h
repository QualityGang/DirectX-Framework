#pragma once

#include "Texture2D.h"

class Sprite
{
public:
	Sprite(const Texture2D& texture);
	virtual ~Sprite() = default;

	void SetPosition(float x, float y);
	void SetSize(float width, float height);
	void SetRotation(float radians);
	void SetPositionAnchor(float x, float y);
	void SetRotationAnchor(float x, float y);
	void SetColor(float r, float g, float b, float a);
	void SetTexture(const Texture2D& texture);
	void SetSourceRect(float x, float y, float width, float height);
	void Move(float x, float y);

	const XMFLOAT2& GetPosition() const;
	const XMFLOAT2& GetSize() const;
	float GetRotation() const;
	const XMFLOAT2& GetPositionAnchor() const;
	const XMFLOAT2& GetRotationAnchor() const;
	const XMFLOAT4& GetColor() const;
	const XMFLOAT4& GetSourceRect() const;

	const Texture2D& GetTexture() const;
private:
	Texture2D m_Texture;
	XMFLOAT2 m_Position;
	XMFLOAT2 m_Size;
	XMFLOAT4 m_Color;
	float m_Rotation;
	XMFLOAT2 m_PositionAnchor;
	XMFLOAT2 m_RotationAnchor;
	XMFLOAT4 m_SourceRect;
};

