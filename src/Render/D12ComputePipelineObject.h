#ifndef RENDER_COMPUTE_PIPELINE_OBJECT_H_INCLUDED
#define RENDER_COMPUTE_PIPELINE_OBJECT_H_INCLUDED

struct ID3D12PipelineState;

namespace gce
{
	class D12PipelineObject;
	class Shader;
	
	class D12ComputePipelineObject
	{
	public:
		D12ComputePipelineObject(Shader const& shader, D12PipelineObject& rootsig);
		D12ComputePipelineObject(D12ComputePipelineObject const& other);
		D12ComputePipelineObject(D12ComputePipelineObject&& other) noexcept = delete;

		D12ComputePipelineObject& operator= (D12ComputePipelineObject const& other);
		D12ComputePipelineObject& operator= (D12ComputePipelineObject&& other) = delete;

		~D12ComputePipelineObject();

	private:
		ID3D12PipelineState* m_pPipelineState = nullptr;

		friend class RenderWindow;
	};

}

#endif