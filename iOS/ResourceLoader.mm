/*
 *  ResourceLoader.h
 *  hex-game
 *
 *  Created by Micke Prag on 2011-08-15.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ResourceLoader.h"
#include "Texture2D.h"

ResourceLoader::ResourceLoader()
:AbstractResourceLoader()
{
}

ResourceLoader::~ResourceLoader() {
}

GLuint ResourceLoader::loadTexture(const std::string &filename) {
	Texture2D *tex = [[Texture2D alloc] initWithImage: [UIImage imageNamed: [NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding] ]]];
	GLuint texRef = [tex name];
	[tex dealloc];
	return texRef;
}
