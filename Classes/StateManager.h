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
#include <string>

class StateManager {
    
    StateManager() {}
    static void createState(Json::Value& root);
    static void loadStateFromFile(Json::Value& root, const std::string& filename);
    static void saveStateToFile(Json::Value& root, const std::string& filename);
    static void recreateFromState(Json::Value& root);

public:
    static void load(const std::string& filename);
    static void save(const std::string& filename);
};

#endif
