
#include "bsp.h"

int main()
{
	BSPH* bsp = new BSPH;

	bsp->open_bsp_source("gm");

	return 0;
}