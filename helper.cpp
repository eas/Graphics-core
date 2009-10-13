#include "stdafx.h"
#include "helper.h"

namespace Helper
{

	ATOM MyRegisterClass( HINSTANCE hInstance, WndProcType wndProc, TCHAR* szWindowClass,
						  unsigned nExtraLong)
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= wndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= sizeof(LONG) * nExtraLong;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= NULL;
		wcex.hCursor		= NULL;
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= szWindowClass;
		wcex.hIconSm		= NULL;

		return RegisterClassEx(&wcex);
	}

	Window::Window(	HINSTANCE hInstance, int nCmdShow, WndProcType wndProc,
					TCHAR* szTitle, TCHAR* szWindowClass, unsigned nExtraLong,
					WindowPostion wPos )
	{
		MyRegisterClass(hInstance, wndProc, szWindowClass, nExtraLong);

		hWnd_ = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			wPos.left, wPos.top, wPos.width, wPos.height, NULL, NULL, hInstance, NULL);

		if (!hWnd_)
		{
			UnregisterClass(szWindowClass, hInstance);
			throw WinApiError( GetLastError() );
		}

		ShowWindow(hWnd_, nCmdShow);
		UpdateWindow(hWnd_);
	}
	Window::~Window()
	{
	}


	const float SpectatorCoords::deltaPhi = D3DX_PI / 24;
	const float SpectatorCoords::deltaTheta = D3DX_PI / 24;
	const float SpectatorCoords::deltaR = 10.0f;
	const float SpectatorCoords::thetaMin = 1e-3f;
	const float SpectatorCoords::thetaMax = D3DX_PI - thetaMin;
	const float SpectatorCoords::rMin = 0.1f;
} // namespace Helper