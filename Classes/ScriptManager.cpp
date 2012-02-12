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
#include "MenuView.h"

#include <sstream>

ScriptManager* ScriptManager::_instance = 0;

ScriptManager* ScriptManager::instance() {
    if (_instance == 0) {
        _instance = new ScriptManager();
    }		
    return _instance;
}

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
}

void ScriptManager::add(std::string& key, ScriptedAction* script) {
    _scriptedActions[key] = script;
    EventManager::instance()->addObserver(script);
}

void ScriptManager::activate(std::string& key) {
    _scriptedActions[key]->doAction();
}

void ScriptManager::clear() {
    EventManager* eventManager = EventManager::instance();
    for (std::map<std::string, ScriptedAction*>::iterator it = _scriptedActions.begin(); it != _scriptedActions.end(); ++it) {
        eventManager->removeObserver(it->second);
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
	SceneLoader::instance()->switchToMenu(new TextboxMenuVC(*this, "YOU GOT 5 SILVER", "OK"));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

void EndBattleSA::callbackVoid() {
	SceneLoader::instance()->returnFromMenu();
    SceneLoader::instance()->returnToAdventureScene();
}



LoadNewBattleSA::LoadNewBattleSA(const ModelEvent& triggerEvent) : ScriptedAction(triggerEvent) {}

void LoadNewBattleSA::doAction() {
    
}