#pragma once

#include "include/Common.h"

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/glut.h>

namespace Moon {

	typedef std::string ContentId;

	enum ContentType {
		NullContent,
		GLFileTexture
	};

	/*
		The Content struct is a generic struct used by many different
		types of loaded content via the ContentProvider. Some members
		will not be used depending on the type of the content.
		Members have comments beside them specifying what type they are for.
		** See ContentType enum **
	*/
	struct Content {
		ContentType type;
		ContentId id;
		GLuint gl_TextureId; /// GLFileTexture
		std::string filePath; /// GLFileTexture
		int width; /// GLFileTexture
		int height; /// GLFileTexture
	};

	class ContentProvider {

		public:
			ContentProvider();
			~ContentProvider() noexcept;

			//Singleton Getter
			static ContentProvider* singleton();

			//Methods
			bool DoesContentExist(ContentId);
			bool IsAssetCached(std::string identifier);
			void FreeContent(ContentId);
			Content Get(ContentId);
			ContentId LoadFileTexture(std::string filePath);

		private:
			//Members
			static ContentProvider* instance;
			std::map<ContentId, Content> loadedContent;
			std::map<std::string, ContentId> loadedAssetsCache;

	};

}