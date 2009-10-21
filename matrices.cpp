#include "stdafx.h"
#include "matrices.h"

const D3DXMATRIX ViewMatrix( const D3DXVECTOR3& eye, const D3DXVECTOR3& at, const D3DXVECTOR3& up )
{
	D3DXVECTOR3 c, _c = at-eye;
	D3DXVec3Normalize( &c, &_c );

	D3DXVECTOR3 a, _a;
	D3DXVec3Cross( &_a, &up, &c );
	D3DXVec3Normalize( &a, &_a );

	D3DXVECTOR3 b;
	D3DXVec3Cross( &b, &c, &a );
	c /= 2.0f;

	D3DXVECTOR3 d(	-D3DXVec3Dot(&eye, &a),
					-D3DXVec3Dot(&eye, &b),
					-D3DXVec3Dot(&eye, &c) );

	return D3DXMATRIX(	a.x,	a.y,	a.z,	d.x,
						b.x,	b.y,	b.z,	d.y,
						c.x,	c.y,	c.z,	d.z,
						0.0f,	0.0f,	0.0f,	1.0f );
}

const D3DXMATRIX ProjectiveMatrix( float front, float back )
{
	float a = back / (back - front);
	float b = back * front / (front - back);
	return D3DXMATRIX(	front,	0.0f,	0.0f,	0.0f,
						0.0f,	front,	0.0f,	0.0f,
						0.0f,	0.0f,	a,		b,
						0.0f,	0.0f,	1.0f,	0.0f );
}