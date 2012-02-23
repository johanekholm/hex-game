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

	std::string actionPyString = ResourceLoader::loadFileAsString("Action.py");
	DEBUGLOG(actionPyString.c_str());
	PyRun_SimpleString(actionPyString.c_str());

	PyObject *main_module, *global_dict, *actionClass, *actionInstance, *returnValue;

	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);

	actionClass = PyDict_GetItemString(global_dict, "BattleActionHeal");

	if (PyCallable_Check(actionClass))
	{
		DEBUGLOG("Callable!");
        actionInstance = PyObject_CallObject(actionClass, NULL);
	}

	returnValue = PyObject_CallMethod(actionInstance, "doAction", NULL);
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



LoadNewBattleSA::LoadNewBattleSA(const ModelEvent& triggerEvent) : ScriptedAction(triggerEvent) {}

void LoadNewBattleSA::doAction() {
    
}