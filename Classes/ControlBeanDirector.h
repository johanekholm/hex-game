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
	ControlBean();
	virtual void start() = 0;
	void setControlBeanDirector(ControlBeanDirector* director);
};

class ControlBeanDirector {
private:
    static ControlBeanDirector* _instance;
	std::vector<ControlBean*> _beans;

    ControlBeanDirector();

public:
    static ControlBeanDirector* instance();
    static void destroy();

	ControlBeanDirector* addBean(ControlBean* bean);
	void beanDidFinish(ControlBean* bean);
};

#endif