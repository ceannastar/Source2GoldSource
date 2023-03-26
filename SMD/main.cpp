
#include "smd.h"

int main()
{

	SMDH* smd = new SMDH;

	smd->write_smd("model");

	return 0;
}