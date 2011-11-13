/*
 *  EventManager.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *
 */

#include "EventManager.h"

EventManager* EventManager::_instance = 0;

void EventManager::destroy() {
	if (_instance != 0) {
		delete _instance;
		_instance=0;
	}
}

EventManager::EventManager() {
    
}

ModelEvent EventManager::getEvent() {
    return _mostRecentEvent;
}

void EventManager::publishEvent(const ModelEvent& event) {
    _mostRecentEvent = event;
    
    DEBUGLOG("Event published: %i", event.type);
    this->updateObservers();
}



