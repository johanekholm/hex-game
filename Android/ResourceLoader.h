/*
 *  ResourceLoader.h
 *  hex-game
 *
 *  Created by Magnus Ahlberg on 2011-08-18.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <jni.h>
#include "AbstractResourceLoader.h"

class ResourceLoader: public AbstractResourceLoader {
public:
    ResourceLoader();
    virtual ~ResourceLoader();
    void setEnv(JNIEnv* env);
    void setGL10(jobject gl10);
    void setCallingObject(jobject callingObject);
protected:
    virtual GLuint loadTexture(const std::string &filename);
private:
	class PrivateData;
	PrivateData *d;
};

