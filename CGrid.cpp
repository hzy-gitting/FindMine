#include "pch.h"
#include "CGrid.h"

CGrid::CGrid()
{
	bLButtonDown = false;
	bFlaged = false;
	attr = EMPTY;
}


bool CGrid::isMine()
{
	return attr == MINE;
}


void CGrid::setNum(int num)
{
	attr = (enum GridAttribute)(NUM1 + num - 1);
}
