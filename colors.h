#pragma once
#include <stdlib.h>

namespace Colors
{
	const DWORD Black =		D3DCOLOR_XRGB(000, 000, 000);
	const DWORD Red =		D3DCOLOR_XRGB(255, 000, 000);
	const DWORD Green =		D3DCOLOR_XRGB(000, 255, 000);
	const DWORD Blue =		D3DCOLOR_XRGB(000, 000, 255);
	const DWORD Yellow =	D3DCOLOR_XRGB(255, 255, 000);
	const DWORD Cyan =		D3DCOLOR_XRGB(000, 255, 255);
	const DWORD Magenta =	D3DCOLOR_XRGB(255, 000, 255);
	const DWORD White =		D3DCOLOR_XRGB(255, 255, 255);
	const DWORD Gray =		D3DCOLOR_XRGB(128, 128, 128);

	inline DWORD Random()
	{
		return D3DCOLOR_XRGB(rand()%256, rand()%256, rand()%256);
	}
} //namespace Colors
