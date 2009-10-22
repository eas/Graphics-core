#pragma once

#include "windows.h"
#include "d3d9.h"
#include "exception"
#include <d3dx9.h>
#include "assert.h"

#ifdef _DEBUG
#pragma comment( lib, "graphics-core-debug.lib" )
#else
#pragma comment( lib, "graphics-core-release.lib" )
#endif


namespace D3D
{
	class Error
		:public std::exception
	{
	public:
		Error(HRESULT errorCode);

		HRESULT errorCode_;
		TCHAR* error_;
	};

	struct DefaultVertex
	{
		float x, y, z;
		DWORD color;

		DefaultVertex() {};
		DefaultVertex( float x, float y, float z, DWORD color)
			:x(x), y(y), z(z), color(color) {}
	};

	typedef UINT Index;
	static const D3DFORMAT indicesFormat = D3DFMT_INDEX32;

	inline void CheckResult(HRESULT errorCode)
	{
		if( D3D_OK != errorCode )
		{
			throw Error(errorCode);
		}
	}

	class GraphicDevice
	{
	public:
		class DC;
		GraphicDevice( HWND hWnd, D3DPRESENT_PARAMETERS& params );
		GraphicDevice( const GraphicDevice& other );

		~GraphicDevice();
		IDirect3DDevice9* GetDevice()
		{
			return device_;
		}
		IDirect3DDevice9* operator->()
		{
			return device_;
		}
		void SetRenderState(D3DRENDERSTATETYPE state, DWORD value)
		{
			CheckResult( device_->SetRenderState(state, value) );
		}
	private:
		IDirect3D9* directX_;
		IDirect3DDevice9* device_;
	};


	class GraphicDevice::DC
	{
	public:
		DC(GraphicDevice& device,
				DWORD flags, D3DCOLOR color, float z, DWORD stencil,
				DWORD count = NULL, const D3DRECT* rects = NULL )
				:device_(device)
		{
			CheckResult( device->Clear( count, rects, flags, color, z, stencil ) );
			CheckResult( device->BeginScene() );
		}
		~DC()
		{
			CheckResult( device_->EndScene() );
			CheckResult( device_->Present( NULL, NULL, NULL, NULL ) );
		}
		void DrawIndexedPrimitive( D3DPRIMITIVETYPE type,INT baseVertexIndex,UINT minIndex, 
									UINT numVertices, UINT startIndex, UINT primitiveCount )
		{
			CheckResult(device_->DrawIndexedPrimitive(type, baseVertexIndex, minIndex,
						numVertices, startIndex, primitiveCount));
		}
	private:
		DC(const DC&);
		DC& operator=(const DC&);

		GraphicDevice device_;
	};

	class Shader
	{
	public:
		Shader(GraphicDevice& device, LPCTSTR fileName);

		void SetConstantF( UINT startRegister, float data );
		IDirect3DVertexShader9* GetShader()
		{
			return shader_;
		}
		void SetMatrix( const D3DXMATRIX& matrix, UINT startRegister )
		{
			CheckResult( device_->SetVertexShaderConstantF( startRegister, matrix, matrixDimension ) );
		}		
		void Use()
		{
			CheckResult( device_->SetVertexShader(shader_) );
		}

		~Shader();
	private:
		Shader(const Shader&);
		Shader& operator=(const Shader&);
	private:
		IDirect3DVertexShader9* shader_;
		GraphicDevice device_;

		static const unsigned matrixDimension = 4;
	};

	template<class Vertex>
	class VertexBuffer
	{
	public:
		VertexBuffer(GraphicDevice& device); // Create object, not buffer
		VertexBuffer(GraphicDevice& device, UINT nVertices);

		~VertexBuffer();
		void SetVertices(const Vertex vertices[], UINT nVertices);
		inline IDirect3DVertexBuffer9* GetBuffer()
		{
			return vertexBuffer_;
		}
		IDirect3DVertexBuffer9* operator->()
		{
			return vertexBuffer_;
		}
		void Use(UINT streamNumber, UINT offsetInBytes)
		{
			CheckResult( device_->SetStreamSource(streamNumber, vertexBuffer_, offsetInBytes, sizeof(Vertex)) );
		}
	private:
		VertexBuffer(const VertexBuffer&);
		VertexBuffer& operator=(const VertexBuffer&);

		IDirect3DVertexBuffer9* vertexBuffer_;
		GraphicDevice device_;
		UINT nVerticesMax_;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(GraphicDevice& device);
		IndexBuffer(GraphicDevice& device, UINT nIndices);
		~IndexBuffer();
		void SetIndices(const Index indices[], UINT nIndices);
		inline IDirect3DIndexBuffer9* GetBuffer()
		{
			return indexBuffer_;
		}
		IDirect3DIndexBuffer9* operator->()
		{
			return indexBuffer_;
		}
		void Use()
		{
			CheckResult( device_->SetIndices(indexBuffer_) );
		}
	private:
		IndexBuffer(const IndexBuffer&);
		IndexBuffer& operator=(const IndexBuffer&);

		IDirect3DIndexBuffer9* indexBuffer_;
		GraphicDevice device_;
		UINT nIndicesMax_;
	};

	class VertexDeclaration
	{
	public:
		VertexDeclaration(GraphicDevice& device, const D3DVERTEXELEMENT9 vertexDeclaration[]);
		~VertexDeclaration();
		IDirect3DVertexDeclaration9* GetDeclaration()
		{
			return vertexDeclaration_;
		}
		void Use()
		{
			CheckResult( device_->SetVertexDeclaration(vertexDeclaration_) );
		}
	private:
		VertexDeclaration(const VertexDeclaration&);
		VertexDeclaration& operator=(const VertexDeclaration&);

		IDirect3DVertexDeclaration9* vertexDeclaration_;
		GraphicDevice device_;
	};


	template<class Vertex>
	VertexBuffer<Vertex>::VertexBuffer(GraphicDevice& device)
		:device_(device), vertexBuffer_(NULL), nVerticesMax_(0)
	{
	}

	template<class Vertex>
	VertexBuffer<Vertex>::VertexBuffer(GraphicDevice& device, UINT nVertices)
		:device_(device), vertexBuffer_(NULL), nVerticesMax_(nVertices)
	{
		CheckResult(device->CreateVertexBuffer( nVertices*sizeof(Vertex),
												D3DUSAGE_WRITEONLY, 0,
												D3DPOOL_DEFAULT, &vertexBuffer_, NULL ));
	}

	template<class Vertex>
	VertexBuffer<Vertex>::~VertexBuffer()
	{
		if( NULL != vertexBuffer_ )
		{
			vertexBuffer_->Release();
		}
	}

	template<class Vertex>
	void VertexBuffer<Vertex>::SetVertices(const Vertex vertices[], UINT nVertices)
	{
		assert( nVertices<=nVerticesMax_ || 0 == nVerticesMax_ );

		if( 0 == nVerticesMax_ )
		{
			CheckResult(device_->CreateVertexBuffer( nVertices*sizeof(Vertex),
												D3DUSAGE_WRITEONLY, 0,
												D3DPOOL_DEFAULT, &vertexBuffer_, NULL ));
		}

		void* buffer = NULL;

		CheckResult( vertexBuffer_->Lock(0, nVertices*sizeof(Vertex), &buffer, 0) );
		memcpy( buffer, vertices, nVertices*sizeof(Vertex));
		CheckResult( vertexBuffer_->Unlock() );
	}
} // namespace D3D