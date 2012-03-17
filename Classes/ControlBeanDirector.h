/*
 *  ControlBeanDirector.h
 *  hex-game
 *
 *  Created by Johan Ekholm.
 *
 */

#ifndef CONTROLBEAN_H
#define CONTROLBEAN_H

#include <vector>

class ControlBeanDirector;

class ControlBean {
protected:
	ControlBeanDirector* _director;
	
public:
	virtual void start() = 0;
};

class ControlBeanDirector {
private:
    static ControlBeanDirector* _instance;
	std::vector<ControlBean*> _beans;

    ControlBeanDirector();

public:
    static ControlBeanDirector* instance();
    static void destroy();

	ControlBeanDirector* add(ControlBean* bean);
	void beanDidFinish(ControlBean* bean);
};

#endif