#pragma once
//格子类
class CGrid
{
public:
	bool bLButtonDown;//是否被左键按下
	bool bFlaged;		//是否被标记
	enum GridAttribute attr;  //格子属性
	CGrid* pRecGrid[8];		//附近格子指针
public:
	CGrid();

	bool isMine();
	void setNum(int num);
};

enum GridAttribute {//格子属性（枚举类型）
	MINE,//地雷
	EMPTY,//空地
	NUM1,//数字1-8
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
}; 