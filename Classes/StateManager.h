/*
 *  StateManager.h
 *
 *  Created by Johan Ekholm
 *
 */

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "json-forwards.h"
#include <vector>
#include <map>
#include <string>

class StateManager {
    static std::map<std::string, std::string> _inMemoryFileStore;
    
    StateManager() {}
    static void createState(Json::Value& root);
    static void saveStateToFile(Json::Value& root, const std::string& filename);
    static void recreateFromState(Json::Value& root);

public:
    static void load(const std::string& filename);
    static void loadStateFromFile(Json::Value& root, const std::string& filename);
    static void save(const std::string& filename);
    static std::map<std::string, std::string> initStaticMap();
};

#endif
