/*
 *  EventManager.h
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *
 */

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include "toolkit.h"
#include "Observable.h"

namespace ModelEventNS {
    const int PARTY_WIPEOUT = 1;
};

struct ModelEvent {
    int type;
    int arg1, arg2;
};

class EventManager : public Observable {
    static EventManager* _instance;
    ModelEvent _mostRecentEvent;
    
    EventManager();

public:
    static EventManager* instance();
    static void destroy();
    void publishEvent(const ModelEvent& event);
    ModelEvent getEvent();
};

#endif