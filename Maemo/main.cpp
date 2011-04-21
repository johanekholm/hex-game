#include "EAGLView.h"
#include <unistd.h>

int main(int argc, char *argv[])
{

	EAGLView l;

	while(1) {
		l.draw();
		sleep(1000);
	}
	return 0;
}
