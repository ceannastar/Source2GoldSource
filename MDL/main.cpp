
#include "mdl.h"

int main()
{

	MDLH* mdl = new MDLH;

	mdl->read_vtx("myfirstmodel");
	//mdl->read_mdl("myfirstmodel");

	return 0;
}