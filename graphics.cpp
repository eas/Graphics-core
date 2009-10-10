#include "stdafx.h"

#include "graphics.h"
#include <tchar.h>


#include "d3dx9.h"
#ifndef NDEBUG
	#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif
namespace D3D
{
	Error::Error(HRESULT errorCode)
		: errorCode_(errorCode)
	{
		switch(errorCode)
		{
		case D3DERR_DEVICELOST:			error_ = _T("D3DERR_DEVICELOST"); break;
		case D3DERR_INVALIDCALL:		error_ = _T("D3DERR_INVALIDCALL"); break;
		case D3DERR_NOTAVAILABLE:		error_ = _T("D3DERR_NOTAVAILABLE"); break;
		case D3DERR_OUTOFVIDEOMEMORY:	error_ = _T("D3DERR_OUTOFVIDEOMEMORY"); break;
		case E_FAIL:					error_ = _T("E_FAIL"); break;
		case D3DXERR_INVALIDDATA:		error_ = _T("D3DXERR_INVALIDDATA"); break;
		case E_OUTOFMEMORY:				error_ = _T("E_OUTOFMEMORY"); break;
		default:
			error_ = _T("Unknown error");
		}
		MessageBox(NULL, error_, NULL, MB_OK);
	}

	const D3DVERTEXELEMENT9 Vertex::vertexDeclaration[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};


	GraphicDevice::GraphicDevice(HWND hWnd, D3DPRESENT_PARAMETERS &params)
		:directX_(NULL),
		device_(NULL)
	{
		if( NULL == ( directX_ = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		{
			throw Error(E_FAIL);
		}
		CheckResult( directX_->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
										  D3DCREATE_HARDWARE_VERTEXPROCESSING,
										  &params, &device_ ) );
	}

	GraphicDevice::GraphicDevice( const GraphicDevice& other )
		:directX_(other.directX_),
		device_(other.device_)
	{
		directX_->AddRef();
		device_->AddRef();
	}

	GraphicDevice::~GraphicDevice()
	{

		if( NULL != device_ )
		{
			device_->Release();
		}

		if( NULL != directX_ )
		{
			directX_->Release();
		}
	}

	Shader::Shader(GraphicDevice& device, LPCTSTR fileName)
		:device_(device), shader_(NULL)
	{
		ID3DXBuffer* shaderCode = NULL;

		CheckResult( D3DXAssembleShaderFromFile(fileName, NULL, NULL, NULL, &shaderCode, NULL) );
		CheckResult( device->CreateVertexShader(static_cast<DWORD*>(shaderCode->GetBufferPointer()), &shader_) );

		shaderCode->Release();
	}

	Shader::~Shader()
	{
		if( NULL != shader_ )
		{
			shader_->Release();
		}
	}

	void Shader::SetWorldMatrix( const D3DXMATRIX& worldMatrix, UINT startRegister )
	{
		worldMatrix_ = worldMatrix;
		SetShaderMatrix(startRegister);
	}
	void Shader::SetViewMatrix( const D3DXMATRIX& viewMatrix, UINT startRegister )
	{
		viewMatrix_ = viewMatrix;
		SetShaderMatrix(startRegister);
	}
	void Shader::SetProjectiveMatrix( const D3DXMATRIX& projectiveMatrix, UINT startRegister )
	{
		projectiveMatrix_ = projectiveMatrix;
		SetShaderMatrix(startRegister);
	}

	void Shader::SetShaderMatrix( UINT startRegister )
	{
		CheckResult( device_->SetVertexShaderConstantF( startRegister, projectiveMatrix_*viewMatrix_*worldMatrix_,
					 matrixDimension ) );
	}


	IndexBuffer::IndexBuffer(GraphicDevice& device, UINT nIndices)
		:device_(device), indexBuffer_(NULL)
	{
		CheckResult(device->CreateIndexBuffer(	nIndices*sizeof(Index),
												D3DUSAGE_WRITEONLY, indicesFormat,
												D3DPOOL_DEFAULT, &indexBuffer_, NULL ));
	}

	IndexBuffer::~IndexBuffer()
	{
		if( NULL != indexBuffer_ )
		{
			indexBuffer_->Release();
		}
	}

	void IndexBuffer::SetIndices(const Index indices[], UINT nIndices)
	{
		void* buffer = NULL;

		CheckResult( indexBuffer_->Lock(0, nIndices*sizeof(Index), &buffer, 0) );
		memcpy( buffer, indices, nIndices*sizeof(Index));
		CheckResult( indexBuffer_->Unlock() );
	}

	VertexBuffer::VertexBuffer(GraphicDevice& device, UINT nVertices)
		:device_(device), vertexBuffer_(NULL)
	{
		CheckResult(device->CreateVertexBuffer( nVertices*sizeof(Vertex),
												D3DUSAGE_WRITEONLY, 0,
												D3DPOOL_DEFAULT, &vertexBuffer_, NULL ));
	}

	VertexBuffer::~VertexBuffer()
	{
		if( NULL != vertexBuffer_ )
		{
			vertexBuffer_->Release();
		}
	}

	void VertexBuffer::SetVertices(const Vertex vertices[], UINT nVertices)
	{
		void* buffer = NULL;

		CheckResult( vertexBuffer_->Lock(0, nVertices*sizeof(Vertex), &buffer, 0) );
		memcpy( buffer, vertices, nVertices*sizeof(Vertex));
		CheckResult( vertexBuffer_->Unlock() );
	}

	VertexDeclaration::VertexDeclaration(GraphicDevice& device)
		:device_(device), vertexDeclaration_(NULL)
	{
		CheckResult( device->CreateVertexDeclaration(Vertex::vertexDeclaration, &vertexDeclaration_) );
	}

	VertexDeclaration::~VertexDeclaration()
	{
		if( NULL != vertexDeclaration_ )
		{
			vertexDeclaration_->Release();
		}
	}
} // namespace D3D