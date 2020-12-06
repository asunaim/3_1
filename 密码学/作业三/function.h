#pragma once




void initializeen(int index);//初始化
void initializede(int index);//初始化

int getbit(int a, int j);//取一个字节中的第几次项位，最高位为第7位
void writebit(int& a, int j);//向一个字节的第几次项位写入数值1
void add(int a, int b, int& c);//c=a+b
void sub(int a, int b, int& c);//c=a-b
int getdeg(int a);//a是几次
void mulbit(int a, int b,  int& r);//r=(a*b)%m,在GF(2^8)下的乘法
int getinv(int a);//求逆元,扩展欧几里得     
void divmod(int a, int b, int& q, int& r);//求商和余数
void divmodm(int a, int& r);//mod x^8+x^4+x^3+x+1
int roundbit(int a);//一个字节右循环移位一位
int change(int x);//仿射变换
void ByteSub();//字节代换
void change1to2(int* a,int **b,int x);//将长为4*x的一维数组排列成二维数组
void change2to1(int* a, int** b, int x);//列数x，二维数组转一维；
void modx41(int& a);//mod x^4+1
void ShiftRow();//行移位,对stateword进行修改
void shiftsinglerow(int a[], int b[], int shift);//将a左移shift位存入b中
void MixColumn();//列混合
void mulbyte(int *a,int *b,int div);//多项式乘法运算,c=a*b
void AddRoundKey(int** roundkey,int round);//密钥加
void KeyExpansion(int* key, int** keyround);//密钥扩展
void selectkey(int** a, int** roundkey,int round);//密钥选取,第i轮
void Round(int** key,int round);//轮加密
void Finalround(int** key);//最后一轮轮加密



void encode();//加密运算
void decode();//解密运算

void InvRound(int** key, int round);
void InvByteSub();//字节代换
void InvMixColumn();//列混合
int Invchange(int x);//仿射变换
int roundbit2(int a);////一个字节右循环移位两位
void InvShiftRow();//行移位,对stateword进行修改
void Invmulbyte(int* a, int* b, int div);//多项式乘法运算,c=a*b
void InvFinalround(int** key);//解密第一轮
void getInvkey(int** expandedkey);//解密密钥
void InvMixColumnkey(int**key);//列混合


