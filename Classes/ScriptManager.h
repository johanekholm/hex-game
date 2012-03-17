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
#include <python.h>
#include "json-forwards.h"

namespace ScriptedActionNS {
    const int END_BATTLE = 1;
    const int NEXT_DUNGEON_ROOM = 2;
};

class ScriptedAction : public IObserver {
    bool _active;
    ModelEvent _triggerEvent;

public:
    static ScriptedAction* build(int actionId, int eventType);
	static ScriptedAction* createScriptedActionFromJson(Json::Value& root);
    ScriptedAction(const ModelEvent& triggerEvent);
    virtual void doAction();
	void updateState();
    void destroyed();
};

class ScriptManager {
public:
    static ScriptManager* instance();
    static void destroy();
    void add(ScriptedAction* script);
    void add(const std::string& key, ScriptedAction* script);
    void activate(std::string& key);
    void clear();
    static PyObject* log(PyObject *self, PyObject *args);
protected:
    ScriptManager();
private:
	class PrivateData;
	PrivateData *d;
};



class EndBattleSA : public ScriptedAction, public ControlCallback {
public:
    EndBattleSA(const ModelEvent& triggerEvent);
    void doAction();
    void callbackVoid();
};

class EndDungeonRoomSA : public ScriptedAction, public ControlCallback {
private:
	std::string _sceneId;
	
public:
    EndDungeonRoomSA(const ModelEvent& triggerEvent);
    EndDungeonRoomSA(const ModelEvent& triggerEvent, Json::Value& root);
    void doAction();    
    virtual void callbackNumber(int num);
};

#endif