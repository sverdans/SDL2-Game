#include <string>
#include <memory>
#include <map>


class ResourceManager final
{
private:
	ResourceManager() = default;
	~ResourceManager() = default;
	
	std::map<std::string, std::shared_ptr<int>> mTexturesMap;
	std::map<std::string, std::shared_ptr<int>> mSpritesMap;

public:
	ResourceManager(const ResourceManager &) = delete;
	ResourceManager(ResourceManager &&) = delete;

	ResourceManager& operator=(const ResourceManager &) = delete;
	ResourceManager& operator=(ResourceManager &&) = delete;

	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	bool Initialize()
	{

	}

	void Finalize()
	{

	}
};