#include "include/Engine/ContentProvider.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

using namespace Moon;

//Constructor
ContentProvider::ContentProvider()
{
	assert(ContentProvider::instance == nullptr); //Make sure we aren't making 2 instances (ContentProvider is a singleton)...
	ContentProvider::instance = this;
}

//Deconstructor
ContentProvider::~ContentProvider()
{
	if (this->loadedContent.size() > 0)
	{
		for (auto iter = this->loadedContent.begin();;)
		{
			this->FreeContent(iter->first);
			iter = this->loadedContent.begin();
			if (this->loadedContent.size() == 0)
			{
				break;
			}
		}
	}
}

//Singleton Getter
ContentProvider* ContentProvider::instance = nullptr;
ContentProvider* ContentProvider::singleton()
{
	assert(ContentProvider::instance != nullptr); //Make sure we never call :singleton() before it's made...
	return ContentProvider::instance;
}

//Methods
bool ContentProvider::DoesContentExist(ContentId id)
{
	return this->loadedContent.find(id) != this->loadedContent.end();
}
bool ContentProvider::IsAssetCached(std::string identifier)
{
	return this->loadedAssetsCache.find(identifier) != this->loadedAssetsCache.end();
}
void ContentProvider::FreeContent(ContentId id)
{
	//Check if content exists
	if (this->DoesContentExist(id))
	{
		Content content = this->Get(id);
		this->loadedContent.erase(id);
		if (content.type == ContentType::GLFileTexture)
		{
			//Cleanup texture
			if (this->IsAssetCached(content.filePath))
			{
				this->loadedAssetsCache.erase(content.filePath);
			}
			glDeleteTextures(1, &content.gl_TextureId);
		}
		StandardOut::Print<std::string>(
			StandardOut::OutputType::Debug,
			"ContentProvider::FreeContent() - Content unloaded (" + id + ")"
		);
	}
	else
	{
		StandardOut::Print<std::string>(
			StandardOut::OutputType::Warning,
			"ContentProvider::FreeContent() - Failed to unload content of id \"" + id + "\" (does not exist)"
		);
	}
}
Content ContentProvider::Get(ContentId id)
{
	//Check if content exists
	if (id == "")
	{
		Content nullContent;
		nullContent.type = ContentType::NullContent;
		return nullContent;
	}
	if (this->DoesContentExist(id))
	{
		return this->loadedContent.at(id);
	}
	else
	{
		StandardOut::Print<std::string>(
			StandardOut::OutputType::Error, 
			"ContentProvider::Get() - Failed to fetch content of id \"" + id + "\"  (does not exist)" 
		);
		Content nullContent;
		nullContent.type = ContentType::NullContent;
		return nullContent;
	}
}
ContentId ContentProvider::LoadFileTexture(std::string filePath)
{
	std::string filePathRaw = filePath;
	#ifdef _DEBUG
		filePath = DEBUG_DIR + filePath;
	#endif
	if (this->IsAssetCached(filePath))
	{
		//Don't load the same texture twice, use the cache!
		return this->loadedAssetsCache.at(filePathRaw);
	}
	else
	{
		//Texture is new, load it in
		int width;
		int height;
		GLuint textureId;
		unsigned char* image;
		if (image = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA))
		{
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image
			);
			SOIL_free_image_data(image);
			Content content;
			content.type = ContentType::GLFileTexture;
			content.id = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
			content.filePath = filePathRaw;
			content.width = width;
			content.height = height;
			content.gl_TextureId = textureId;
			this->loadedContent.emplace(content.id, content);
			this->loadedAssetsCache.emplace(content.filePath, content.id);
			StandardOut::Print<std::string>(
				StandardOut::OutputType::Debug,
				"ContentProvider::LoadFileTexture() - Loaded content \"" + filePathRaw + "\" (" + content.id + ")"
			);
			return content.id;
		}
		else
		{
			StandardOut::Print<std::string>(
				StandardOut::OutputType::Error,
				"ContentProvider::LoadFileTexture() - Failed to load content \"" + filePathRaw + "\""
			);
			SOIL_free_image_data(image);
			return std::string("");
		}
	}
}
