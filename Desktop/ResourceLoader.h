/*
 *  ResourceLoader.h
 *  hex-game
 *
 *  Created by Micke Prag on 2011-08-15.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AbstractResourceLoader.h"

class EAGLView;

class ResourceLoader: public AbstractResourceLoader {
public:
    ResourceLoader(EAGLView *view);
    virtual ~ResourceLoader();
    static std::string loadLuaScript(const std::string &scriptName);
protected:
    virtual GLuint loadTexture(const std::string &filename);

private:
    class PrivateData;
    PrivateData *d;
};

