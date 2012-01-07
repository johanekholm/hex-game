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

ScriptManager* ScriptManager::_instance = 0;

void ScriptManager::destroy() {
	if (_instance != 0) {
       	for (std::map<std::string, ScriptedAction*>::iterator it = _instance->_scriptedActions.begin(); it != _instance->_scriptedActions.end(); ++it) {
            delete it->second;
        }
		_instance->_scriptedActions.clear();
        
		delete _instance;
		_instance=0;
	}
}

ScriptManager::ScriptManager() {
    
}

void ScriptManager::add(ScriptedAction* script) {
    std::stringstream stream;
    stream << "DEF-" << _scriptedActions.size();

    _scriptedActions[stream.str()] = script;
    EventManager::instance()->addObserver(script);
    DEBUGLOG("Script registered");
}

void ScriptManager::add(std::string& key, ScriptedAction* script) {
    _scriptedActions[key] = script;
    EventManager::instance()->addObserver(script);
    DEBUGLOG("Script registered: %s", key.c_str());
}

void ScriptManager::activate(std::string& key) {
    _scriptedActions[key]->doAction();
}

void ScriptManager::clear() {
    for (std::map<std::string, ScriptedAction*>::iterator it = _scriptedActions.begin(); it != _scriptedActions.end(); ++it) {
        delete it->second;
    }
    _scriptedActions.clear();
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
    //SceneLoader::instance()->switchToAdventureScene();
    SceneLoader::instance()->loadPrevious();
}



LoadNewBattleSA::LoadNewBattleSA(const ModelEvent& triggerEvent) : ScriptedAction(triggerEvent) {}

void LoadNewBattleSA::doAction() {
    
}