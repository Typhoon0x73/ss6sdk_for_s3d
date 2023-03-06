
# pragma once
# include <Siv3D.hpp>

namespace s3d
{
	class SSAnimePack
	{
	public:

		SSAnimePack() = default;

		~SSAnimePack() = default;

		explicit SSAnimePack(FilePathView ssae, bool createEditorParam);

		bool load(FilePathView ssae, bool createEditorParam);

	private:
	};
}
