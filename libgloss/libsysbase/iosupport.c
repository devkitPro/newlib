#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/iosupport.h>

static int defaultDevice = -1;

//---------------------------------------------------------------------------------
void setDefaultDevice( int device ) {
//---------------------------------------------------------------------------------

	if ( device >2 && device <= STD_MAX)
		defaultDevice = device;
}

//---------------------------------------------------------------------------------
static ssize_t null_write(struct _reent *r,void *fd,const char *ptr, size_t len) {
//---------------------------------------------------------------------------------
	return len;
}

//---------------------------------------------------------------------------------
const devoptab_t dotab_stdnull = {
//---------------------------------------------------------------------------------
	.name         = "stdnull",
	.structSize   = 0,
	.write_r      = null_write,
	.dirStateSize = 0
};

//---------------------------------------------------------------------------------
const devoptab_t *devoptab_list[STD_MAX] = {
//---------------------------------------------------------------------------------
	&dotab_stdnull, &dotab_stdnull, &dotab_stdnull
};

//---------------------------------------------------------------------------------
int FindDevice(const char* name) {
//---------------------------------------------------------------------------------
	int i = 0, namelen, dev_namelen, dev = -1;
	char *separator;

	separator = strchr(name, ':');

	if (separator == NULL) return defaultDevice;

	dev_namelen = separator - name;

	while(i<STD_MAX) {
		if(devoptab_list[i]) {
			namelen = strlen(devoptab_list[i]->name);
			if(dev_namelen == namelen && strncmp(devoptab_list[i]->name,name,namelen)==0 ) {
				dev = i;
				break;
			}
		}
		i++;
	}

	return dev;
}

//---------------------------------------------------------------------------------
int RemoveDevice( const char* name) {
//---------------------------------------------------------------------------------
	int dev = FindDevice(name);

	if ( -1 != dev ) {
		devoptab_list[dev] = NULL;
		return 0;
	}

	return -1;

}

//---------------------------------------------------------------------------------
int AddDevice( const devoptab_t* device) {
//---------------------------------------------------------------------------------

	int devnum;

	for ( devnum = 3;devnum <STD_MAX; devnum++ ) {
		// null pointer is available
		if ( devoptab_list[devnum] == NULL ) break;

		if ( !strcmp(devoptab_list[devnum]->name, device->name) && strlen(devoptab_list[devnum]->name) == strlen(device->name) )
			break;
	}

	if ( devnum == STD_MAX ) {
		devnum = -1;
	} else {
		devoptab_list[devnum] = device;
	}
	return devnum;
}

//---------------------------------------------------------------------------------
const devoptab_t* GetDeviceOpTab (const char *name) {
//---------------------------------------------------------------------------------
	int dev = FindDevice(name);
	if (dev >= 0 && dev < STD_MAX) {
		return devoptab_list[dev];
	} else {
		return NULL;
	}
}
