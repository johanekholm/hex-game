/*
 *  ScriptManager.h
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *
 */

#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <map>
#include "toolkit.h"
#include "EventManager.h"
#include "IObserver.h"
#include "ControlCallback.h"

namespace ScriptedActionNS {
    const int END_BATTLE = 1;
    const int LOAD_BATTLE = 2;
};

class ScriptedAction : public IObserver {
    ModelEvent _triggerEvent;
    
public:
    static ScriptedAction* build(int actionId, int eventType);
    ScriptedAction(const ModelEvent& triggerEvent);
    virtual void doAction();
	void updateState();
    void destroyed();
};

class ScriptManager {
    static ScriptManager* _instance;
    std::map<std::string, ScriptedAction*> _scriptedActions;
    
    ScriptManager();

public:
    static ScriptManager* instance() {
		if (_instance == 0) {
			_instance = new ScriptManager();
		}		
		return _instance;
	}
    
    static void destroy();
    void add(ScriptedAction* script);
    void add(std::string& key, ScriptedAction* script);
    void activate(std::string& key);
};



class EndBattleSA : public ScriptedAction, public ControlCallback {
public:
    EndBattleSA(const ModelEvent& triggerEvent);
    void doAction();
    void callbackVoid();
};

class LoadNewBattleSA : public ScriptedAction {
public:
    LoadNewBattleSA(const ModelEvent& triggerEvent);
    void doAction();    
};

#endif