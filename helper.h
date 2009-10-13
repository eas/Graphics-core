#pragma once
#include "exception"
#include "windows.h"
#include "d3dx9.h"

namespace Helper
{

	class WinApiError
		:public std::exception
	{
	public:
		WinApiError( DWORD errorCode )
			: errorCode_(errorCode)
		{
		}

	public:
		DWORD errorCode_;
	};

	struct WindowPostion
	{
		int height;
		int width;
		int left;
		int top;
	};

	const WindowPostion defaultWindowPostion = { 500, 500, 350, 120 };

	typedef LRESULT (CALLBACK *WndProcType)(HWND, UINT, WPARAM, LPARAM);

	class Window
	{
	public:
		Window(	HINSTANCE hInstance, int nCmdShow, WndProcType wndProc,
				TCHAR* szTitle, TCHAR* szWindowClass, unsigned nExtraLong,
				WindowPostion windowPosition = defaultWindowPostion );
		~Window();
		HWND GetHWND()
		{
			return hWnd_;
		}

	private:
		Window(const Window&);
		Window& operator=(const Window&);

		HWND hWnd_;
	};

	class SpectatorCoords
	{
	public:
		SpectatorCoords(float r, float theta, float phi)
			:r_(r), theta_(theta), phi_(phi)
		{
		}
		const D3DXVECTOR3 GetCartesianCoords() const
		{
			return D3DXVECTOR3(	r_*sinf(theta_)*cosf(phi_),
								r_*cosf(theta_),
								r_*sinf(theta_)*sinf(phi_) );
		}
		void IncTheta() { if((theta_+=deltaTheta) > thetaMax){ theta_= thetaMax; } }
		void DecTheta() { if((theta_-=deltaTheta) < thetaMin){ theta_= thetaMin; } }
		void IncPhi() { phi_ += deltaPhi; }
		void DecPhi() { phi_ -= deltaPhi; }
		void IncR() { r_ += deltaR; }
		void DecR() { if((r_-=deltaR) < rMin){ r_ = rMin; } }


	private:
		float r_, theta_, phi_;

	public:
		static const float deltaPhi;
		static const float deltaTheta;
		static const float deltaR;
		static const float thetaMin;
		static const float thetaMax;
		static const float rMin;
	};

} // namespace Helper