#pragma once

#include "common.h"
#include "vertexlayout.h"
#include "../datalist.h"
#include "device.h"

namespace cbs
{
	class VertexShader: public AutoRelease<ID3D11VertexShader>
	{
	private:
		AutoRelease<ID3D11InputLayout> m_layout;

		inline void _create(DataList<BYTE> data, DataList<D3D11_INPUT_ELEMENT_DESC> desc)
		{
			assert(desc.size() <= 0xffffffff);

			throwhr(g_device->CreateVertexShader(data.begin(), data.size(), nullptr, &m_ptr));
			throwhr(g_device->CreateInputLayout(desc.begin(), (UINT)desc.size(), data.begin(), data.size(), &m_layout));
		}
		inline void _create(DataList<BYTE> data, DataList<VertexLayout> vl)
		{
			AutoDeleteArray<D3D11_INPUT_ELEMENT_DESC> ildesc;
			ildesc.allocate(vl.size());

			unsigned int offset = 0;
			for (size_t i = 0; i < vl.size(); i++)
			{
				const VertexLayout &d = vl[i];
				D3D11_INPUT_ELEMENT_DESC & ie = ildesc[i];
				switch (d.semantic)
				{
				case VertexLayout::BiNormal:		ie.SemanticName = "BINORMAL"; break;
				case VertexLayout::BlendIndices:	ie.SemanticName = "BLENDINDICES"; break;
				case VertexLayout::BlendWeight:		ie.SemanticName = "BLENDWEIGHT"; break;
				case VertexLayout::Color:			ie.SemanticName = "COLOR"; break;
				case VertexLayout::Normal:			ie.SemanticName = "NORMAL"; break;
				case VertexLayout::Position:		ie.SemanticName = "POSITION"; break;
				case VertexLayout::PositionT:		ie.SemanticName = "POSITIONT"; break;
				case VertexLayout::PSize:			ie.SemanticName = "PSIZE"; break;
				case VertexLayout::Tangent:			ie.SemanticName = "TANGENT"; break;
				case VertexLayout::Texcoord:		ie.SemanticName = "TEXCOORD"; break;
				default: assert(!"Unknown blend setting");
				}
				VertexFormatInfo finfo(d.format);

				ie.SemanticIndex = d.slot;
				ie.Format = d.format;
				ie.InputSlot = 0;
				ie.AlignedByteOffset = offset;
				ie.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				ie.InstanceDataStepRate = 0;
				offset += finfo.byteSize;
			}
			_create(data, DataList<D3D11_INPUT_ELEMENT_DESC>(ildesc, vl.size()));
		}

	public:
		inline VertexShader()
		{

		}
		inline VertexShader(DataList<BYTE> data, DataList<D3D11_INPUT_ELEMENT_DESC> desc)
		{
			_create(data, desc);
		}
		inline VertexShader(DataList<BYTE> data, DataList<VertexLayout> vl)
		{
			_create(data, vl);
		}
		inline ID3D11InputLayout * getInputLayer()
		{
			return m_layout;
		}
	};
	class PixelShader : public AutoRelease<ID3D11PixelShader>
	{
	private:
		inline void _create(DataList<BYTE> data)
		{
			throwhr(g_device->CreatePixelShader(data.begin(), data.size(), nullptr, &m_ptr));
		}

	public:
		inline PixelShader()
		{

		}
		inline explicit PixelShader(DataList<BYTE> data)
		{
			_create(data);
		}
	};

}