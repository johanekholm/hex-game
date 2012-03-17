/*
 *  ControlBeanDirector.cpp
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *
 */

#include "ControlBeanDirector.h"

ControlBeanDirector* ControlBeanDirector::_instance = 0;

ControlBeanDirector* ControlBeanDirector::instance() {
    if (_instance == 0) {
        _instance = new ControlBeanDirector();
    }		
    return _instance;
}

void ControlBeanDirector::destroy() {
	if (_instance != 0) {
        for (std::vector<ControlBean*>::iterator it = _instance->_beans.begin(); it != _instance->_beans.end(); ++it) {
            delete *it;
        }
        
		_instance->_beans.clear();
		_instance=0;
	}
}

ControlBeanDirector::ControlBeanDirector() {
}

ControlBeanDirector* ControlBeanDirector::add(ControlBean* bean) {
    _beans.push_back(bean);
	
	// if first bean then start directly
	if (_beans.size() == 1) {
		bean->start();
	}
	
	return this;
}

void ControlBeanDirector::beanDidFinish(ControlBean* bean) {
	// remove bean and start next in line
	if (bean == _beans.front()) {
		_beans.erase(_beans.begin());
		delete bean;
		if (_beans.size() > 0) {
			_beans.front()->start();
		}
	}
}