
# include "SSProject.hpp"

namespace s3d
{

	SSProject::SSProject(FilePathView path)
		: SSProject()
	{
		load(path);
	}

	bool SSProject::load(FilePathView path)
	{
		XMLReader reader{ path };
		return true;
	}
}
