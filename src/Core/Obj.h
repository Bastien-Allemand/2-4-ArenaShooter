#ifndef CORE_OBJ_H_INCLUDED
#define CORE_OBJ_H_INCLUDED

#include <Containers/Vector.hpp>
#include "define.h"
#include "Maths/Vector2.hpp" 
#include "Maths/Vector3.h" 

namespace gce
{
	class File;

	namespace obj
	{
		struct Vertex
		{
			Vector3f32 position; // v
			Vector3f32 normal; // vn
			Vector2f32 textureCoordinate; // vt
		};
	}
	struct Mesh
	{
		Vector <uint32> indices;
	};
	struct MTLData
	{
		String mtlId;
		uint32 mtlIndexBegin;
		uint32 mtlIndexEnd;
		
	};
	struct Obj
	{
	public:
		Obj(String const& path);
		Obj(String const&& path);
		~Obj();

		void LoadObj();

		Vector<Mesh> meshs;
		Vector<obj::Vertex> vertices;
		Vector<MTLData> mtlData;
		bool mtlExists = false;

	private:
		void FreeVectors();
		File* pFile;
		String m_path;
		Vector<Vector3f32> m_tempPosVect;
		Vector<Vector2f32> m_tempCoordVect;
		Vector<Vector3f32> m_tempNormVect;
		Vector<uint32> m_tempIndiceVect;
	};
}
#endif
