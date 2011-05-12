#ifndef GROUP_H
#define GROUP_H

typedef struct Group
{
	int iIdGroup;
	GArray* aObjects;
	GArray* aGroups;
}Group;


Group* Group_newGroup();



#endif
