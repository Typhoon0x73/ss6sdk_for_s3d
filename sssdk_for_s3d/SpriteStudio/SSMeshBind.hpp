#pragma once
#ifndef SS_MESH_BIND_H_
#define SS_MESH_BIND_H_

#include <Siv3D.hpp>

namespace sssdk
{
	class SSMeshBind
	{
	public:

		struct Info
		{
			int32 boneIndex;
			int32 weight;
			Float3 offset;
		};

		explicit SSMeshBind();
		explicit SSMeshBind(const String& line);
		virtual ~SSMeshBind();

		bool load(const String& line);

		SIV3D_NODISCARD_CXX20 const Array<SSMeshBind::Info>& getInfomations() const;

	private:

		Array<Info> m_infos;
	};
}

#endif // !SS_MESH_BIND_H_
