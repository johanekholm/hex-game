//
//  EAGLView.h
//  Tutorial1
//
//  Created by Michael Daley on 25/02/2009.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#include "toolkit.h"
#include <string>


class EAGLView {

public:
	EAGLView();
	~EAGLView();

	void draw();

protected:
	GLuint loadTexture(const std::string &filename);
	
private:
	class PrivateData;
	PrivateData *d;
};
