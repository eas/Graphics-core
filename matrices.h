#pragma once
#include "d3dx9.h"

inline const D3DXMATRIX UnityMatrix()
{
	return D3DXMATRIX(	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f );
}

inline const D3DXMATRIX ScaleMatrix( float scaleX, float scaleY, float scaleZ )
{
	return D3DXMATRIX( scaleX,		0.0f,		0.0f,		0.0f,
					   0.0f,		scaleY,		0.0f,		0.0f,
					   0.0f,		0.0f,		scaleZ,		0.0f,
					   0.0f,		0.0f,		0.0f,		1.0f );
}

inline const D3DXMATRIX ScaleMatrix( const D3DXVECTOR3& scale )
{
	return ScaleMatrix( scale.x, scale.y, scale.z );
}

inline const D3DXMATRIX TranslationMatrix( float dx, float dy, float dz )
{
	return D3DXMATRIX( 1.0f,	0.0f,	0.0f,	dx,
					   0.0f,	1.0f,	0.0f,	dy,
					   0.0f,	0.0f,	1.0f,	dz,
					   0.0f,	0.0f,	0.0f,	1.0f );
}
inline const D3DXMATRIX TranslationMatrix( const D3DXVECTOR3& translation )
{
	return TranslationMatrix( translation.x, translation.y, translation.z );
}
inline const D3DXMATRIX RotateZMatrix( float angle )
{
	return D3DXMATRIX(  cosf(angle),	sinf(angle),	0.0f,		0.0f,
					   -sinf(angle),	cosf(angle),	0.0f,		0.0f,
						0.0f,			0.0f,			1.0f,		0.0f,
						0.0f,			0.0f,			0.0f,		1.0f );
}

inline const D3DXMATRIX RotateYMatrix( float angle )
{
	return D3DXMATRIX(  cosf(angle),	0.0f,		-sinf(angle),	0.0f,
						0.0f,			1.0f,		0.0f,			0.0f,
						sinf(angle),	0.0f,		cosf(angle),	0.0f,
						0.0f,			0.0f,		0.0f,			1.0f );
}

inline const D3DXMATRIX RotateXMatrix( float angle )
{
	return D3DXMATRIX(  1.0f,		0.0f,			0.0f,			0.0f,
						0.0f,		cosf(angle),	sinf(angle),	0.0f,
						0.0f,		-sinf(angle),	cosf(angle),	0.0f,
						0.0f,		0.0f,			0.0f,			1.0f );
}

const D3DXMATRIX ViewMatrix( const D3DXVECTOR3& eye, const D3DXVECTOR3& at, const D3DXVECTOR3& up );

const D3DXMATRIX ProjectiveMatrix( float front, float back );

