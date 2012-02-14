/*
 *  TextureCatalog.mm
 *  hex-game
 *
 *  Created by Johan Ekholm on 2011-04-10.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TextureCatalog.h"
#include "TextureMap.h"

/*private TextureCatalog::TextureCatalog() {
	
}*/

TextureCatalog* TextureCatalog::_instance = 0;

TextureCatalog* TextureCatalog::instance() {
    if (_instance == 0) {
        _instance = new TextureCatalog();
    }
    
    return _instance;
}

void TextureCatalog::destroy() {
	if (_instance != 0) {
		_instance->_textureMaps.clear();
		delete _instance;
		_instance=0;
	}
}

void TextureCatalog::add(TextureMap* texMap, const std::string& name) {
	_textureMaps[name] = texMap;
}

void TextureCatalog::addAndLoad(const std::string& name, GLuint texture, int numDivs) {
	TextureMap* newTex = new TextureMap(texture, numDivs);
	
	_textureMaps[name] = newTex;
}

TextureMap* TextureCatalog::get(const std::string& name) {
	return _textureMaps[name];
}


