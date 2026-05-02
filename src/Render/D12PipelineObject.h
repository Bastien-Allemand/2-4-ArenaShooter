#ifndef RENDER_PIPELINE_OBJECT_H_INLCUDED
#define RENDER_PIPELINE_OBJECT_H_INLCUDED

#include <d3d12.h>

#include "Core/define.h"

namespace gce
{
	class Shader;

	enum PSOFlag : uint8
	{
		FLAG_NONE,
		FLAG_SKYBOX = 0b00000001,
		FLAG_WIREFRAME = 0b00000010,
	};

	inline PSOFlag operator|(PSOFlag f1, PSOFlag f2) { return static_cast<PSOFlag>(static_cast<uint8>(f1) | static_cast<uint8>(f2)); }

	class D12PipelineObject
	{
	public:
		D12PipelineObject() = default;
		D12PipelineObject(Shader const& VS, Shader const& PS, Shader const& HS, Shader const& DS, Shader const& rootsig, PSOFlag flag = FLAG_NONE);
		D12PipelineObject(D12PipelineObject const& other) = delete;
		D12PipelineObject(D12PipelineObject&& other) noexcept = delete;

		D12PipelineObject& operator=(D12PipelineObject const& other) = delete;
		D12PipelineObject& operator=(D12PipelineObject&& other) = delete;

		~D12PipelineObject();
		D12PipelineObject(Shader const& VS, Shader const& PS, Shader const& rootsig, PSOFlag flag = FLAG_NONE);

	protected:
		void Init(Shader const& VS, Shader const& PS, Shader const& rootsig, PSOFlag flag = FLAG_NONE);

	private:
		ID3D12PipelineState* m_pPipelineState = nullptr;
		ID3D12RootSignature* m_pRootSignature = nullptr;

		D3D12_INPUT_ELEMENT_DESC m_inputLayout[4]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		friend class CubeMap;
		friend class RenderWindow;
		friend class RenderTarget;
		friend class D12ComputePipelineObject;
	};
	
}
#endif