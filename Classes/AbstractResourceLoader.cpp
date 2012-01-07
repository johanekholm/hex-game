/*
 *  AbstractResourceLoader.h
 *  hex-game
 *
 *  Created by Micke Prag on 2011-08-15.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AbstractResourceLoader.h"
#include "TextureCatalog.h"

AbstractResourceLoader::AbstractResourceLoader() {
    
}

AbstractResourceLoader::~AbstractResourceLoader() {
    
}

void AbstractResourceLoader::load() {
    TextureCatalog* catalog = TextureCatalog::instance();
    
    catalog->addAndLoad("hexTiles", this->loadTexture("texmap_hex.png"), 4);
    catalog->addAndLoad("actions", this->loadTexture("actions.png"), 4);
    catalog->addAndLoad("icons", this->loadTexture("icons.png"), 4);
    catalog->addAndLoad("units", this->loadTexture("game_objects.png"), 4);
    catalog->addAndLoad("font", this->loadTexture("font_1.png"), 1);

}
