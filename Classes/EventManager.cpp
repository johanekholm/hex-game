/*
 *  EventManager.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *
 */

#include "EventManager.h"

EventManager* EventManager::_instance = 0;

EventManager* EventManager::instance() {
    if (_instance == 0) {
        _instance = new EventManager();
    }		
    return _instance;
}

void EventManager::destroy() {
	if (_instance != 0) {
		delete _instance;
		_instance=0;
	}
}

EventManager::EventManager() {
    _mostRecentEvent.type = -1;
}

ModelEvent EventManager::getEvent() {
    return _mostRecentEvent;
}

void EventManager::publishEvent(const ModelEvent& event) {
    _mostRecentEvent = event;
    
    this->updateObservers();
    _mostRecentEvent.type = -1;
}



