#pragma once
//������
class CGrid
{
public:
	bool bLButtonDown;//�Ƿ��������
	bool bFlaged;		//�Ƿ񱻱��
	enum GridAttribute attr;  //��������
	CGrid* pRecGrid[8];		//��������ָ��
public:
	CGrid();

	bool isMine();
	void setNum(int num);
};

enum GridAttribute {//�������ԣ�ö�����ͣ�
	MINE,//����
	EMPTY,//�յ�
	NUM1,//����1-8
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
}; 