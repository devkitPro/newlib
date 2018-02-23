#include <unistd.h>

struct __argv {
	int argvMagic;		//!< argv magic number, set to 0x5f617267 ('_arg') if valid 
	char *commandLine;	//!< base address of command line, set of null terminated strings
	int length;//!< total length of command line
	int argc;
	char **argv;
};

extern struct __argv *__system_argv;
void __init();
void SYS_PreMain();

void __crtmain() {
	SYS_PreMain();
	exit ( main(__system_argv->argc,__system_argv->argv) );
}
