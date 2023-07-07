
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
		if (lines.isEmpty())
		{
			return false;
		}
		for (size_t i = 0; (i + 4) < lines.size(); i += 4)
		{
			int32 boneIndex = ParseOr<int32, int32>(lines[i + 1], 0);
			int32 weight = ParseOr<int32, int32>(lines[i + 2], 0);
			double x = ParseOr<double, double>(lines[i + 3], 0.0);
			double y = ParseOr<double, double>(lines[i + 4], 0.0);
			m_infos.emplace_back(boneIndex, weight, Vec2{ x, y });
		}
		return true;
	}

	const Array<SSMeshBind::Info>& SSMeshBind::getInfomations() const
	{
		return m_infos;
	}
}
