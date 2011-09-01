/*
 *  ResourceLoader.h
 *  hex-game
 *
 *  Created by Micke Prag on 2011-08-15.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ResourceLoader.h"
#include "EAGLView.h"

class ResourceLoader::PrivateData {
public:
    EAGLView *view;
};

ResourceLoader::ResourceLoader(EAGLView *view)
:AbstractResourceLoader(), d(new PrivateData())
{
    d->view = view;
}

ResourceLoader::~ResourceLoader() {
    delete d;
}

GLuint ResourceLoader::loadTexture(const std::string &filename) {
    QString path = QString(":/Resources/") + filename.c_str();
	return d->view->bindTexture(QPixmap(path), GL_TEXTURE_2D, GL_RGBA, QGLContext::NoBindOption);	
}

std::string ResourceLoader::loadLuaScript(const std::string &filename) {
    //TODO: Implement
}