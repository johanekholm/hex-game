/*
 *  AbstractResourceLoader.h
 *  hex-game
 *
 *  Created by Micke Prag on 2011-08-15.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "toolkit.h"
#include <string>

namespace DirNS {
    const int RESOURCE  = 1;
    const int SAVE		= 2;
};

class AbstractResourceLoader {
public:
    AbstractResourceLoader();
    virtual ~AbstractResourceLoader();
    
    void load();
	static bool doesFileExist(const std::string& fileName, int dir);
    static std::string loadFileAsString(const std::string &fileName);
    static std::string loadFileAsString(const std::string &fileName, int dir);
	static void setWorkingDirectory(const std::string& path);
    static void writeStringToFile(const std::string& fileName, int dir);

protected:
    virtual GLuint loadTexture(const std::string &filename) = 0;
};
