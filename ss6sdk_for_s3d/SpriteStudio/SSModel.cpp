
#include "SSModel.hpp"

namespace sssdk
{
	SSModel::SSModel()
		: m_parts{}
		, m_bones{}
		, m_meshBinds{}
	{
	}

	SSModel::SSModel(const XMLElement& element)
	{
		load(element);
	}

	SSModel::~SSModel()
	{
	}

	bool SSModel::load(const XMLElement& element)
	{
		if (element.name() != U"Model")
		{
			return false;
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"partList")
			{
				for (auto part = child.firstChild(); part; part = part.nextSibling())
				{
					m_parts.emplace_back(part);
				}
			}
			else if (name == U"boneList")
			{
				for (auto bone = child.firstChild(); bone; bone = bone.nextSibling())
				{
					m_bones.emplace_back(bone);
				}
			}
			else if (name == U"meshList")
			{
				for (auto mesh = child.firstChild(); mesh; mesh = mesh.nextSibling())
				{
					const auto& lines = mesh.text().split(U',');
					for (const auto& line : lines)
					{
						m_meshBinds.emplace_back(line);
					}
				}
			}
		}
		return true;
	}

	const Array<SSModelPart>& SSModel::getParts() const
	{
		return m_parts;
	}

	const SSModelPart* const SSModel::getPart(int32 index) const
	{
		if (index < 0 or index >= m_parts.size())
		{
			return nullptr;
		}
		return &m_parts[index];
	}

	const SSModelPart* const SSModel::getPart(StringView name) const
	{
		for (const auto& part : m_parts)
		{
			if (part.getName() == name)
			{
				return &part;
			}
		}
		return nullptr;
	}

	const Array<SSBoneItem>& SSModel::getBones() const
	{
		return m_bones;
	}

	const Array<SSMeshBind>& SSModel::getMeshBinds() const
	{
		return m_meshBinds;
	}
}
