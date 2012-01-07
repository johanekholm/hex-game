/*
 *  ScriptManager.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *
 */

#include "ScriptManager.h"
#include "CentralControl.h"
#include "EventManager.h"
#include "SceneLoader.h"
#include "TransitionViewController.h"

#include <sstream>

class ScriptManager::PrivateData {
public:
    std::map<std::string, ScriptedAction*> scriptedActions;
	static ScriptManager* instance;
};

ScriptManager* ScriptManager::PrivateData::instance = 0;

ScriptManager* ScriptManager::instance() {
	if (PrivateData::instance == 0) {
		PrivateData::instance = new ScriptManager();
	}
	return PrivateData::instance;
}

void ScriptManager::destroy() {
	if (PrivateData::instance != 0) {
		for (std::map<std::string, ScriptedAction*>::iterator it = PrivateData::instance->d->scriptedActions.begin(); it != PrivateData::instance->d->scriptedActions.end(); ++it) {
            delete it->second;
        }
		PrivateData::instance->d->scriptedActions.clear();
        
		delete PrivateData::instance;
		PrivateData::instance=0;
	}
}

ScriptManager::ScriptManager() {
	d = new PrivateData;
}

void ScriptManager::add(ScriptedAction* script) {
    std::stringstream stream;
    stream << "DEF-" << d->scriptedActions.size();

    d->scriptedActions[stream.str()] = script;
    EventManager::instance()->addObserver(script);
}

void ScriptManager::add(std::string& key, ScriptedAction* script) {
    d->scriptedActions[key] = script;
    EventManager::instance()->addObserver(script);
}

void ScriptManager::activate(std::string& key) {
    d->scriptedActions[key]->doAction();
}


ScriptedAction* ScriptedAction::build(int actionId, int eventType) {
    using namespace ScriptedActionNS;
    
    ModelEvent event;
    event.type = eventType;
    
    switch (actionId) {
		case END_BATTLE:
            return new EndBattleSA(event);
		case LOAD_BATTLE:
            return new LoadNewBattleSA(event);
            
		default:
            return 0;
    }
    
}

ScriptedAction::ScriptedAction(const ModelEvent& triggerEvent) {
    _active = true;
    _triggerEvent = triggerEvent;
}

void ScriptedAction::doAction() {}

void ScriptedAction::updateState() {
    if (_active && EventManager::instance()->getEvent().type == _triggerEvent.type) {
        _active = false;
        this->doAction();
    }
}

void ScriptedAction::destroyed() {}




EndBattleSA::EndBattleSA(const ModelEvent& triggerEvent) : ScriptedAction(triggerEvent) {}

void EndBattleSA::doAction() {
    DEBUGLOG("Ending battle - Fade out");
    SceneLoader::instance()->switchToTransition(new FadeOutTransition(*this));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

void EndBattleSA::callbackVoid() {
    DEBUGLOG("Ending battle - Load scene");
    //CentralControl::instance()->switchMode(ControlMode::ADVENTURE);
    SceneLoader::instance()->switchToAdventureScene();    
}



LoadNewBattleSA::LoadNewBattleSA(const ModelEvent& triggerEvent) : ScriptedAction(triggerEvent) {}

void LoadNewBattleSA::doAction() {
    
}