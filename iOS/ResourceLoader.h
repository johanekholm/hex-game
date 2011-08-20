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
protected:
    virtual GLuint loadTexture(const std::string &filename);
};

