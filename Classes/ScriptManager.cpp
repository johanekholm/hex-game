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
#include "ResourceLoader.h"
#include "ModelManager.h"
#include "MapObject.h"
#include "Item.h"

#include "json.h"
#include <sstream>
#include <python.h>

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

		Py_Finalize();

		delete PrivateData::instance;
		PrivateData::instance=0;
	}
}

ScriptManager::ScriptManager() {
	d = new PrivateData;
	//Python
	Py_SetProgramName((char*)"HexGame");

	Py_Initialize();

	std::string pyString = ResourceLoader::loadFileAsString("test.py");
	PyMethodDef LogMethods[] = {
	   {"log",  ScriptManager::log, METH_VARARGS,
	    "Execute a shell command."},
	   {NULL, NULL, 0, NULL}        /* Sentinel */
	};

	PyObject *m;
	m = Py_InitModule("debuglog", LogMethods);

	PyRun_SimpleString(pyString.c_str());
}

/* Temporary function to log from Python */
PyObject* ScriptManager::log(PyObject *self, PyObject *args)
{
	const char *logString;

	if (!PyArg_ParseTuple(args, "s", &logString))
       return NULL;
	DEBUGLOG("%s", logString);
	return NULL;
/*   sts = system(command);
   return Py_BuildValue("i", sts);*/
}

void ScriptManager::add(ScriptedAction* script) {
    std::stringstream stream;
    stream << "DEF-" << d->scriptedActions.size();

	this->add(stream.str(), script);
}

void ScriptManager::add(const std::string& key, ScriptedAction* script) {
	if (script != 0) {
		d->scriptedActions[key] = script;
		EventManager::instance()->addObserver(script);
	}
}

void ScriptManager::activate(std::string& key) {
    d->scriptedActions[key]->doAction();
}

void ScriptManager::clear() {
    EventManager* eventManager = EventManager::instance();
    for (std::map<std::string, ScriptedAction*>::iterator it = d->scriptedActions.begin(); it != d->scriptedActions.end(); ++it) {
        eventManager->removeObserver(it->second);
        delete it->second;
    }
    d->scriptedActions.clear();
}

ScriptedAction* ScriptedAction::build(int actionId, int eventType) {
    using namespace ScriptedActionNS;

    ModelEvent event;
    event.type = eventType;

    switch (actionId) {
		case END_BATTLE:
            return new EndBattleSA(event);
		case NEXT_DUNGEON_ROOM:
            return new EndDungeonRoomSA(event);
            
		default:
            return 0;
    }
}

ScriptedAction* ScriptedAction::createScriptedActionFromJson(Json::Value& root) {
	ScriptedAction* action;
	std::string actionId = root.get("actionId", "MISSING").asString();
	
    ModelEvent event;
    event.type = root.get("eventId", 0).asInt();

	if (actionId == "END_BATTLE") {
		action = new EndBattleSA(event);
	} else if (actionId == "NEXT_DUNGEON_ROOM") {
		action = new EndDungeonRoomSA(event, root);
	}
	return action;
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
	MapObject* player = ModelManager::instance()->getFirstMapObjectWithOwner(FactionNS::PLAYER);
	
	if (player != 0) {
		player->addItem(new Item(ItemNS::SILVER, 5));
	}

	SceneLoader::instance()->switchToMenu(new TextboxMenuVC(*this, "YOU GOT 5 SILVER", "OK"));
    CentralControl::instance()->switchMode(ControlMode::MENU);
}

void EndBattleSA::callbackVoid() {
	SceneLoader::instance()->returnFromMenu();
    SceneLoader::instance()->returnToAdventureScene();
}



EndDungeonRoomSA::EndDungeonRoomSA(const ModelEvent& triggerEvent) : ScriptedAction(triggerEvent) {}

EndDungeonRoomSA::EndDungeonRoomSA(const ModelEvent& triggerEvent, Json::Value& root) : ScriptedAction(triggerEvent) {
	_sceneId = root.get("sceneId", "MISSING").asString();
}

void EndDungeonRoomSA::doAction() {
    std::vector<MenuChoice> choices;
	choices.push_back(MenuChoice::makeChoice(1, "CONTINUE"));
	choices.push_back(MenuChoice::makeChoice(2, "EXIT DUNGEON"));
    
    SceneLoader::instance()->switchToMenu(new ChoiceMenuVC(*this, choices));
}

void EndDungeonRoomSA::callbackNumber(int num) {
	SceneLoader::instance()->returnFromMenu();
	switch (num) {
		case 1:
			SceneLoader::instance()->loadNextDungeonScene(_sceneId, 0);			
			break;
			
		case 2:
			SceneLoader::instance()->returnToAdventureScene();
			break;
			
		default:
			break;
	}
}
