/*
 *  SceneContext.h
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "json-forwards.h"

class SceneContext {
private:
	class PrivateData;
	PrivateData* d;
    SceneContext();

public:
	static SceneContext* instance();
    static void destroy();
	
    Json::Value serialize();
    void deserialize(Json::Value& root);

	void setPartyId1(int partyId);
	void setPartyId2(int partyId);
	
	int getPartyId1();
	int getPartyId2();
};

