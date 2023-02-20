
# include "SSExternalTextures.hpp"

namespace s3d
{
	SSExternalTextures::SSExternalTextures(const XMLElement& ExternalTextures, bool createEditorParam)
	{
		load(ExternalTextures, createEditorParam);
	}

	bool SSExternalTextures::load(const XMLElement& ExternalTextures, bool createEditorParam)
	{
		return true;
	}
}
