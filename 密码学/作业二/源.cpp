#include"head.h"
#include"testData.h"
int test(BYTE a[8], BYTE b[8])//�Ա����������Ԫ���Ƿ���ͬ
{
	int flag = 1;
	for (int i = 0; i < 8; i++)
		if (a[i] != b[i])
		{
			flag = 0;
			break;
		}
	return flag;
}

void funcEn(des_test_case m)//���ܹ���
{
	BYTE key_final[16][6] = {};
	//������Կ
	getkeys(m.key, key_final);
	BYTE cipher[8] = {};
	round(m.txt, key_final, cipher);
	cout<<test(m.out, cipher)<<endl;
}

void funcDe(des_test_case c)
{
	BYTE key_final_[16][6] = {};
	BYTE key_final[16][6] = {};
	getkeys(c.key, key_final_);
	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 6; j++)
			key_final[15 - i][j] = key_final_[i][j];
	BYTE m[8] = {};
	round(c.txt, key_final, m);
	cout << test(c.out, m) << endl;
}

int main()
{
	for (int i = 0; i < 20; i++)
	{
		if (cases[i].mode == 1)
		{
			cout << "����" << cases[i].num << endl;
			funcEn(cases[i]);
			cout << "=====================" << endl;
		}
		else 
		{ 
			cout << "����" << cases[i].num << endl;
			funcDe(cases[i]); 
			cout << "=====================" << endl;
		}
	}
}