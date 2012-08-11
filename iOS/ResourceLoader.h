/*
 *  ResourceLoader.h
 *  hex-game
 *
 *  Created by Micke Prag on 2011-08-15.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AbstractResourceLoader.h"

class ResourceLoader: public AbstractResourceLoader {
public:
    ResourceLoader();
    virtual ~ResourceLoader();
	static bool doesFileExist(const std::string& fileName, int dir);
	static std::string loadFileAsString(const std::string &filename);
    static std::string loadFileAsString(const std::string &filename, int dir);
    static void writeStringToFile(const std::string& textString, const std::string& filename, int dir);

protected:
	static NSString* getPathToDir(int dir);
    virtual GLuint loadTexture(const std::string &filename);
};

