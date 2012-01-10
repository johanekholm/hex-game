/*
 *  ScriptManager.h
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *
 */

#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <string>
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
    bool _active;
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
    static ScriptManager* instance();
    static void destroy();
    void activate(std::string& key);
    void add(ScriptedAction* script);
    void add(std::string& key, ScriptedAction* script);
    void clear();
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