#pragma once
#include <stdlib.h>
#include <d3dx9.h>

namespace Colors
{
	const D3DXCOLOR Black	=		D3DCOLOR_XRGB(000, 000, 000);
	const D3DXCOLOR Red		=		D3DCOLOR_XRGB(255, 000, 000);
	const D3DXCOLOR Green	=		D3DCOLOR_XRGB(000, 255, 000);
	const D3DXCOLOR Blue	=		D3DCOLOR_XRGB(000, 000, 255);
	const D3DXCOLOR Yellow	=		D3DCOLOR_XRGB(255, 255, 000);
	const D3DXCOLOR Cyan	=		D3DCOLOR_XRGB(000, 255, 255);
	const D3DXCOLOR Magenta	=		D3DCOLOR_XRGB(255, 000, 255);
	const D3DXCOLOR White	=		D3DCOLOR_XRGB(255, 255, 255);
	const D3DXCOLOR Gray	=		D3DCOLOR_XRGB(128, 128, 128);



	inline D3DXCOLOR Random()
	{
		return D3DCOLOR_XRGB(rand()%256, rand()%256, rand()%256);
	}
} //namespace Colors
