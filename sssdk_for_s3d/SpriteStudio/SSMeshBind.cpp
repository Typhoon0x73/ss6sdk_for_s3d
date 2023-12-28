
#include "SSMeshBind.hpp"

namespace sssdk
{

	SSMeshBind::SSMeshBind()
		: m_infos{}
	{
	}

	SSMeshBind::SSMeshBind(const String& line)
		: SSMeshBind()
	{
		load(line);
	}

	SSMeshBind::~SSMeshBind()
	{
	}

	bool SSMeshBind::load(const String& line)
	{
		const auto& lines = line.split(U' ');
		// 空の場合もある
		if (lines.isEmpty())
		{
			return true;
		}
		for (size_t i = 0; (i + 4) < lines.size(); i += 4)
		{
			int32 boneIndex = ParseOr<int32, int32>(lines[i + 1], 0);
			int32 weight = ParseOr<int32, int32>(lines[i + 2], 0);
			float x = ParseOr<float, float>(lines[i + 3], 0.0f);
			float y = ParseOr<float, float>(lines[i + 4], 0.0f);
			m_infos.emplace_back(boneIndex, weight, Float3{ x, y, 0.0f });
		}
		return true;
	}

	const Array<SSMeshBind::Info>& SSMeshBind::getInfomations() const
	{
		return m_infos;
	}
}
