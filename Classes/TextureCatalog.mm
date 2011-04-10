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

void TextureCatalog::destroy() {

	_textureMaps.clear();
	
	delete _instance;
	_instance=0;
}

void TextureCatalog::add(TextureMap* texMap, const std::string& name) {
	_textureMaps[name] = texMap;
}

void TextureCatalog::addAndLoad(const std::string& name, const std::string& filename, int numDivs) {
	TextureMap* newTex = new TextureMap(filename, numDivs);
	
	_textureMaps[name] = newTex;
}

TextureMap* TextureCatalog::get(const std::string name) {
	return _textureMaps[name];
}


