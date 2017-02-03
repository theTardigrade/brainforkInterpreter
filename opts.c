#include "bfi.h"


Opts globalOptions;

void initGlobalOptions()
{
	Opts *o = &globalOptions;

    o->legacy = false;
	o->maxFileSize = DEFAULT_MAX_FILE_SIZE;
}
