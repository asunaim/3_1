#include "getprime.h"


int main()
{
	//big a, b,n,c;
	//a.set(67863);
	//a.num[100] = 1;
	//b.set(2341);
	////n.num[2] = 3;
	////c = mod(n, a);
	//prime s(1);
	//c = getinv(a, b);
	//cout << "b1=";
	//c.print();
	//cout << "a=";
	//a.print();
	//cout << "b=";
	//b.print();
	//prime sp;
	//int j;

	big p, q;
	p.set(7); q.set(17);
	RSA_ a(p, q);
	big m;
	m.set(19);
	RSAen_ temp(a, m);
	temp.c.print();

	RSAde_ temp_(a, temp.c);
	temp_.m.print();
}

prime::prime(int x)
{
	getarbitrary();

}


void big::copy(big a)
{
	for (int i = 0; i < 256; i++)
		num[i] = a.num[i];
}



big::big() {
	memset(this, 0, sizeof(big));
}
void big::print()
{
	int i;
	for (i = 255; i > 0; i--)
		if (num[i])
			break;
	cout << "0x";
	for (; i >= 0; i--)
	{
		//cout << hex<<num[i];
		printf("%x", num[i]);
		if (i % 4 == 0)
			cout << "";
	}
	cout << endl;
}

big pow(big a, big b, big n)//a^b mod n
{
	big buffer[256];
	//buffer[0].num[0]=1;
	buffer[0].copy(a);

	int x;
	for (x = 255; x > 0; x--)
		if (b.num[x])
			break;
	for (int i = 1; i <= x; i++)
	{
		big tempbuf[5];
		tempbuf[0].copy(buffer[i - 1]);
		for (int j = 1; j < 5; j++)
		{
			big temp = mul(tempbuf[j - 1], tempbuf[j - 1]);
			tempbuf[j] = mod(temp, n);
		}
		buffer[i].copy(tempbuf[4]);
	}

	big product;
	product.num[0] = 1;

	for (int i = 255; i >= 0; i--)
	{
		big temp;
		for(int j=0;j<b.num[i];j++)
		{
			temp=mul(product, buffer[i]);
			product=mod(temp,n);
			//product.print();
		}
	}

	return product;
}






big add(big a, big b)
{
	big c;
	for (int i = 0; i < 256; i++)
	{
		c.num[i] = a.num[i] + b.num[i];
	}
	for (int i = 0; i < 255; i++)
	{
		//int t = c.num[i] % 16;
		c.num[i + 1] += c.num[i]/16;
		c.num[i] %= 16;
	}
	return c;
}
big sub(big a, big b)
{
	big c;
	for (int i = 0; i < 256; i++)
	{
		c.num[i] = a.num[i] - b.num[i];
	}
	for (int i = 0; i < 255; i++)
	{
		//int t = c.num[i] % 16;
		if (c.num[i] < 0)
		{
			c.num[i] += 16;
			c.num[i+1] -= 1;
		}
	}
	return c;
}
big sub(big a, big b,int d)
{
	big c;
	c.copy(a);
	for (int i = d; i < 256; i++)
	{
		c.num[i] -= b.num[i-d];
	}
	for (int i = 0; i < 255; i++)
	{
		//int t = c.num[i] % 16;
		if (c.num[i] < 0)
		{
			c.num[i] += 16;
			c.num[i + 1] -= 1;
		}
	}
	return c;
}

big mul(big a, big b)
{
	big c;
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			c.num[i + j] += a.num[i] * b.num[j];
		}
	}
	for (int i = 0; i < 254; i++)
	{
		//int t = c.num[i] % 16;
		c.num[i + 1] += c.num[i] / 16;
		c.num[i] %= 16;
	}
	return c;
}

big div(big a, big b, big& c)
{
	

	big q;
	int t = compare(a, b);
	if (t == -1)return q;
	if (t == 0)
	{
		big c;
		c.set(1);
		return c;//0
	}

	int x, y;
	for (x = 255; x >= 0; x--)
		if (a.num[x])
			break;
	for (y = 255; y >= 0; y--)
		if (b.num[y])
			break;
	int d = x - y;

	c.copy(a);
	//c.print();
	while (d >= 0)
	{
		while (c.num[x] > b.num[y])
		{
			c = sub(c, b, d);
			big v1;
			v1.num[d]=1;
			q=add(q,v1);
			//c.print();
		}
		if (c.num[x] == b.num[y] && compare(c, b, d) == 1)
		{
			c = sub(c, b, d);
			big v1;
			v1.num[d] = 1;
			q = add(q, v1);
		}
		d--;
		//if (d < 0&&c.num[x]==0)break;
		int t;
		while (compare(c, b) == 1 && c.num[x])
		{
			c = sub(c, b, d);
			big v1;
			v1.num[d] = 1;
			q = add(q, v1);
			//c.print();
		}
		if (c.num[x] == 0)x--;
		t = compare(c, b);
		if (t == -1)break;
		if (t == 0)
		{
			c = sub(c, b);
			big v1;
			v1.num[d] = 1;
			q = add(q, v1);
			return q;
		}
	}
	return q;

}


big mod(big a, big b)//a>b
{
	int t = compare(a, b);
	if (t==-1)return a;
	if (t == 0)
	{
		big c;
		return c;//0
	}
	
	int x, y;
	for (x = 255; x >= 0; x--)
		if (a.num[x])
			break;
	for (y = 255; y >= 0; y--)
		if (b.num[y])
			break;
	int d = x - y;

	big c;
	c.copy(a);
	//c.print();
	while (d >= 0)
	{
		while (c.num[x] > b.num[y])
		{
			c = sub(c, b, d);
			//c.print();
		}
		if(c.num[x]==b.num[y]&& compare(c, b,d) == 1)
			c = sub(c, b, d);
		d--;
		//if (d < 0&&c.num[x]==0)break;
		int t;
		while (compare(c, b) == 1 && c.num[x])
		{
			c = sub(c, b, d);
			//c.print();
		}
		if (c.num[x] == 0)x--;
		t = compare(c, b);
		if (t == -1)return c;
		if (t == 0)
		{
			big s;
			return s;
		}
	}
	return c;
}

int compare(big a, big b)
{
	for (int i = 255; i >= 0; i--)
	{
		if (a.num[i] > b.num[i])return 1;
		if (a.num[i] < b.num[i])return -1;

	}
	return 0;
}
int compare(big a, big b,int d)
{
	for (int i = 255; i >= d; i--)
	{
		if (a.num[i] > b.num[i-d])return 1;
		if (a.num[i] < b.num[i-d])return -1;

	}
	return 0;
}

big getinv(big a, big b) //a mod b
{
	b = mod(b, a);
	big q[100], t[100],r[100];
	r[0].copy(a);
	r[1].copy(b);
	t[0].set(0);
	t[1].set(1);
	int i;
	big v1;
	v1.set(1);
	for(i=2;i<100;i++)
	{
		q[i - 1] = div(r[i - 2], r[i - 1], r[i]);
		big temp = mul(q[i - 1], t[i - 1]);
		while (compare(temp, t[i - 2]) == 1)
			t[i - 2] = add(t[i - 2], r[0]);
		t[i] = sub(t[i - 2], temp);
		if (compare(r[i], v1) == 0)
			break;
	}
	return t[i];
}



void getsmallprime()
{
	//找到2000内的素数
	int a[10001];
	memset(a, 0, sizeof(a));
	for (int i = 2; i <= 100; i++)
	{
		for (int j = 2; i * j <= 10000; j++)
		{
			a[i * j] = 1;
		}
	}
	int index = 0;
	for (int i = 2; i <=10000; i++)
	{
		if (!a[i])
			smallprime[index++] = i;
	}
}





void prime::getarbitrary()
{
	unsigned long long num[16];
	//生成512bit随机数
	srand((unsigned)time(NULL));
	num[0] = rand()%(INT_MAX-1)+1;//随机生成x0

	num[0] = 5;

	for (int i = 1; i < 16; i++)
	{
		num[i] = (num[i - 1] * LINEAR_A) % LINEAR_M;
	}
	num[15] |= (1 << 31)&0x0000000011111111;
	num[0] |= 1;//最高位和最低为置1
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			number.num[i * 8 + j] = num[i] % 16;
			num[i] /= 16;
		}
	}
	int i;
}

void big::set(unsigned int a)
{
	memset(this, 0, sizeof(big));
	int i = 0;
	while (a)
	{
		num[i] = a % 16;
		a /= 16;
		i++;
	}
}
void big::set(unsigned long long a)
{
	memset(this, 0, sizeof(big));
	int i = 0;
	while (a)
	{
		num[i] = a % 16;
		a /= 16;
		i++;
	}
}
void big::set(int a)
{
	memset(this, 0, sizeof(big));
	if (a < 0)return;
	int i = 0;
	while (a)
	{
		num[i] = a % 16;
		a /= 16;
		i++;
	}
}

int big::getbit(int i)
{
	if ((num[i / 4] & (1 << (i % 4))) == 0)
		return 0;
	return 1;
}




prime::prime()
{
	memset(this, 0, sizeof(prime));
	getarbitrary();//生成512bit随机数，奇数
	check2000();//检查是否能被2000以内的素数整除,若不能则加2，直至可以
	millerrabin();//检查是否是素数
}



void prime::check2000()//检查是否能被2000以内的素数整除,若不能则加2，直至可以
{
	if (smallprime[0] == 0)
		getsmallprime();
	big v0;//0
	big v2;
	v2.set(2);
	while (1)
	{
		int flag = 1;
		for (int i = 0; i < 303; i++)
		{
			big temp;
			temp.set(smallprime[i]);
			big s = mod(number, temp);
			if (compare(s, v0) == 0)
			{
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			break;
		}
		else
		{
			number = add(number, v2);
		}
	}
}
 
void prime::millerrabin()//检查是否是素数,512位！！！
{
	int a[5] = { 2,3,13,17,19 };
	int flag =0;
	big n1;
	big v1,v2;
	v1.set(1);
	v2.set(2);
	big v0;
	v0.set(0);

	int high;



	//for (int j = 10; j < 1000; j++)
	{
		//number.set(smallprime[j]);
		while (!flag)
		{
			flag = 1;
			n1 = sub(number, v1);
			for (high = 127; high >= 0; high--)
			{
				if (number.num[high])
					break;
			}
			big temp, d;
			
			for (int k = 0; k < 5; k++)
			{
				cout << "--------------" << endl << a[k] << ":" << endl;
				temp.set(a[k]);
				d.set(1);
				for (int i = high * 4 + 3; i >= 0; i--)
				{
					cout << i << endl;
					big x;
					x.copy(d);
					d = mul(d, d);
					d = mod(d, number);
					d.print();
					if (compare(d, v1) == 0)
					{
						if (compare(x, v1) && compare(x, n1))
						{
							flag = 0;
							break;
						}
					}
					if (n1.getbit(i))
					{

						d = mul(d, temp);
						d = mod(d, number);
						d.print();
					}

				}
				if (compare(d, v1))
				{
					flag = 0;
					number.print();
				}
				if (!flag)break;
			}

			if (!flag)
			{
				number = add(number, v2);
				check2000();
				number.print();
			}
		}
	}
	int h;
	//bigint n1;
	//n1.copy(number);
	//n1.sub(1);//n-1
	//int flag = 0;
	//while (!flag)
	//{
	//	flag = 1;
	//	int h = 0;
	//	unsigned int a[5] = { 2,3,6,8,13 };
	//	for (; h < 4; h++)//5次
	//	{
	//		//srand((unsigned)time(NULL));
	//		//unsigned int a = rand()%(n1.num[0]-3)+2;//随机生成a
	//		bigint a_;
	//		a_.num[0] = a[h];

	//		bigint d;

	//		d.num[0] = 1;
	//		for (int i = 511; i >= 0; i--)
	//		{
	//			bigint x;
	//			x.copy(d);
	//			bigint temp;
	//			bigint::mul(d, d, temp);
	//			bigint::mod(temp, number, d);

	//			if (d.equal(1) && !x.equal(1) && !bigint::compare(x, n1))//x!=1和n-1
	//			{
	//				flag = 0;
	//				break;
	//			}
	//			if (n1.getbit(i))
	//			{
	//				bigint temp;
	//				bigint::mul(d, a_, temp);
	//				bigint::mod(temp, number, d);
	//			}
	//		}
	//		if (!d.equal(1))
	//		{
	//			flag = 0;
	//			break;
	//		}
	//		else flag = 1;
	//	}
	//	if (flag == 0)
	//	{
	//		number.add(2);
	//		check2000();
	//		h = 0;
	//	}
	//}
}




RSA_::RSA_(big p, big q)
{
	big v1;
	v1.set(1);

	this->p = p;
	this->q = q;

	n = mul(p, q);
	big p1, q1;
	p1 = sub(p, v1);
	q1 = sub(q, v1);

	phi = mul(p1, q1);
	e.set(5);
	d = getinv(phi, e);
}


RSAen_::RSAen_(RSA_ a, big m)
{
	this->n = a.n;
	this->e = a.e;
	this->m = m;

	c = pow(m, e, n);
}

RSAde_::RSAde_(RSA_ a, big c)
{
	this->n = a.n;
	this->d = a.d;
	this->c = c;

	m = pow(c, d, n);
}










/////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////






bigint::bigint() { 
	//deg = 0; //num = new unsigned int[64];
	memset(num, 0, sizeof(num));
}


//bigint::bigint(int x)
//{
//	//num = new unsigned int[x];
//	memset(num, 0, sizeof(num));
//	deg = x;
//}


void bigint::in(string a)//16进制输入
{
	//没有0x
	memset(this, 0, sizeof(this));
	string t = "0";
	int len = t.length();
	for (int i = 0; i < 64-len; i++)
		t += "0";
	t += a;

	for (int i = 0; i<16; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->num[i] = this->num[i] * 16 + t[i * 4 + j];
		}
	}
}

int change(char a)
{
	if (a >= '0' && a <= '9')
		return (int)(a - 48);
	switch (a)
	{
	case 'a':
	case 'A':
		return 10;
	case 'b' :
	case 'B':
		return 11;
	case 'c':
	case 'C':
		return 12;
	case 'd':
	case  'D':
		return 13;
	case 'e':
	case  'E':
		return 14;
	case 'f':
	case 'F':
		return 15;
	default:
		break;
	}
}




void bigint::copy(bigint a)//深拷贝
{
	//if (deg == 0) deg = a.deg;
	for (int i = 0; i < 64; i++)
	{
		num[i] = a.num[i];
	}
}

void bigint::writebit(int x, int y)//向二进制的第x次项写入y
{
	if (!getbit(x) && y == 1)
	{
		num[x/32] += pow(2, x % 32);
	}
	else if (getbit(x) && y == 0)
		num[x / 32] -= pow(2, x % 32);
}

int bigint::getbit(int x)//取二进制的第x次项
{
	int t = x / 32;
	if(num[t])
	if (num[t] & (unsigned int)pow(2, x%32))
		return 1;
	return 0;
}



unsigned long long  bigint::getint(unsigned int x, unsigned int y)//取出大整数的第x次项到第y次项  0<=（y-x）<32
{
	bigint temp;
	if (x < 0)x = 0;
	for (int i = x; i <= y; i++)
	{
		if(this->num[i/32])//非0
		if (getbit(i))
		{
			temp.writebit(i-x, 1);
		}
	}
	return temp.num[0];
}

void bigint::div(unsigned int b, bigint& q, bigint &r)//num%b==r
{
	r.copy(*this);
	for (int i = 30; i >= 0; i--)
	{
		//if (r.getbit(i))
		{
			//int s = (i - 31) < 0 ? 0 : (i - 31);
			unsigned long long m= ((unsigned long long)r.num[i+1])*pow(2,32)+r.num[i];
			if (m >= b)
			{
				bigint t;
				t.num[0] = m / b;
				q.writenum(t, i * 32);
				
				r.num[i] = 0; r.num[1] = 0;
				t.num[0] = m % b;
				r.writenum(t, i * 32);
			}
			//else break;
		}
	}
}

void bigint::writenum(bigint b,int j)//最低项第j次项写入数b
{
	//int d = b.deg;
	if (j < 0)j = 0;
	for (int i = j; i <= j+31; i++)
	{
		int temp = b.getbit(i - j);
		writebit(i, temp);
	}
}

void bigint::sub(unsigned int x, int j)//num的第j次项为最低项，j+31最高
{
	unsigned int temp = getint(j, j + 31);
	if (temp > x)
	{
		temp %= x;
		bigint t;
		t.num[0] = temp;
		writenum(t, j);
	}
}




void bigint::sub(unsigned int x)//减法
{
	bigint temp;
	if (num[0] > x)//不用借位
	{
		num[0] -= x;
		return;
	}
	num[0] = INT_MAX - x;
	for (int i = 1; i < 64; i++)
	{
		if (num[i] >= 1)
		{
			num[i] -= 1;
			return;
		}
		num[i] = INT_MAX - 1;
	}
}




void bigint::add(unsigned int x)//+x
{
	if (num[0] < INT_MAX - x)
	{
		//不用进位
		num[0] += x;
		return;
	}
	num[0] += x;//进位
	for (int i = 1; i < 64; i++)
	{
		if (num[i] < INT_MAX - 1)
		{
			num[i] += 1;
			return;
		}
		num[i] = 0;
	}
}



void bigint::add(bigint a, bigint b, bigint& c)
{
	c.copy(a);
	for (int i = 0; i < 64; i++)
	{
		if (c.num[i] >= INT_MAX - b.num[i])//进位
		{
			for (int j = i + 1; j < 64; j++)
				if (c.num[j] < INT_MAX - 1)
				{
					c.num[j] += 1;
					break;
				}
				else {
					c.num[j] = 0;
				}
		}
		c.num[i] = c.num[i] + b.num[i];
	}
}

void bigint:: sub(bigint a, bigint b, bigint& c)//c=a-b
{
	c.copy(a);
	for (int i = 0; i < 64; i++)
	{
		if (c.num[i] < b.num[i])//借位
		{
			for (int j = i + 1; j < 64; j++)
			{
				if (c.num[j])//不是0
				{
					c.num[j] -= 1;
					break;
				}
				c.num[j] = INT_MAX-1;
			}
		}
		c.num[i] -= b.num[i];
	}
}

int bigint::compare(bigint a, bigint b)//a>b 1，a=b 0, a<b -1
{
	for (int i = 63; i >= 0; i--)
	{
		if (a.num[i] > b.num[i]) return 1;
		if (a.num[i] < b.num[i]) return -1;
	}
	return 0;
}

void bigint::mod(bigint a, unsigned int  b, bigint& c)
{
	c.copy(a);
	int i;
	for (i = 63; i >= 0; i--)
	{
		if (a.num[i])
			break;
	}
	unsigned long long w = 0;
	while (i>=0)
	{
		w = ((unsigned long long)c.num[i]) & (0x00000000ffffffff);
		w+=((unsigned long long)c.num[i + 1] << 32) & 0xffffffff00000000;
		w %= b;
		c.num[i + 1] = 0;
		c.num[i] = w;
		//a.num[i] =(w+a.num[i]&0x0000000011111111)% b;
		//w = (unsigned long long)a.num[i];
		//w <<= 32;
		i--;
	}
}
void bigint::mod(bigint a, bigint b, bigint& c)//c=a%b
{
	c.copy(a);
	/*bigint t;
	div(a, b, t, c);*/
	if (compare(a, b) == -1)
		return;



	int x, y;
	for (x = 63; x >= 0; x--)
		if (a.num[x])
			break;
	for (y = 63; y >= 0; y--)
		if (b.num[y])
			break;

	bigint* temp = new bigint[x - y + 1];
	temp[0].copy(b);
	for (int i = 1; i < x - y + 1; i++)
		temp[i - 1].leftmove4(temp[i]);

	unsigned long long w = 0;
	while (compare(c,b)>=0)//c>=b
	{
		if (c.num[x]||w!=0)
		{
			//unsigned int s = c.num[x] / (b.num[y]);
			w += c.num[x];
			unsigned long long s = w / (b.num[y]);
			if (s)
			{
				bigint s_;
				mul(temp[x - y], s, s_);
				if (compare(s_, c) == 1)
				{
					s /= 2;
					if(s)
						mul(temp[x - y], s, s_);
					//temp[x - y].print();
					//s_.print();
				}
				
				//if (x == 0x15)
				//	temp[x - y].print();
				if(s)sub(c, s_, c);
				else w = (unsigned long long)c.num[x] * pow(2, 32);
				if(!(w&0xffffffff00000000))
					w = 0;
			}
			else
			{
			w = (unsigned long long)c.num[x] * pow(2, 32);
				x--;
			}
		}
		else x--;
		if (x < y)break;
	}

	//for (int i = a.deg - 1; i >= b.deg; i--)
	//	c.num[i] = 0;
	/*for (int i = 32 * b.deg - 1; i >= 0; i--)
	{
		if (!b.getbit(i) && c.getbit(i))
		{
			c.writebit(i, 0);
			bigint temp;
			temp.copy(b);
		}
		if (b.getbit(i)) break;
	}*/
	//if (compare(a, b) == -1)
	//	return;

	//bigint temp[32];
	//temp[0].copy(b);
	//for (int i = 1; i < 32; i++)
	//	temp[i-1].leftmove(temp[i]);
	//for (int i = 31; i >= 0; i--)
	//{
	//	while (compare(c, temp[i]) == 1)//a>b
	//	{
	//		sub(c, temp[i], c);

	//	}
	//}
	//bigint t;
	//a.div(b, t, c);
}

void bigint::leftmove4(bigint& temp)//左移四位
{
	for (int i = 0; i < 63; i++)
	{
		temp.num[i + 1] = this->num[i];
	}
}
void bigint::mul(bigint a, bigint b, bigint& c)//c=a*b
{
	memset(&c, 0, sizeof(bigint));
	unsigned long long temp;
	for (int i = 0; i < 32; i++)
	{
		if(a.num[i])
		for (int j = 0; j < 32; j++)
		{
			temp = (unsigned long long)a.num[i] * (unsigned long long)b.num[j];
			bigint s;
			s.num[i + j] = temp;
			s.num[i + j + 1] = temp >> 32;
			add(c, s, c);
		}
	}
}


void bigint::mul(bigint a, unsigned long long b, bigint& c)//c=a*b
{
	memset(&c, 0, sizeof(bigint));
	unsigned long long temp;
	for (int i = 0; i < 63; i++)
	{
		unsigned long long t = b%0x100000000;
		if (t == 0)break;
		unsigned long long m = (unsigned long long)a.num[i];
		temp = m * t;
		if (temp)
		{
			bigint s;
			s.num[i] = temp;
			if(i+1<=63)
			s.num[i + 1] = temp >> 32;
			add(c, s, c);
		}
	}
	for (int i = 0; i < 62; i++)
	{
		unsigned long long t = b & 0xffffffff00000000;
		if (t == 0)break;
		unsigned long long m = (unsigned long long)a.num[i];
		temp = (unsigned long long)a.num[i] * t;
		if (temp)
		{
			bigint s;
			s.num[i+1] = temp;
			if(i+2<=63)
			s.num[i + 2] = temp >> 32;
			add(c, s, c);
		}
	}
}


void bigint::leftmove(bigint& temp)//左移
{
	//temp.deg = deg;
	int highest[64];//存储最高位
	for (int i = 0; i < 64; i++)
	{
		highest[i] = getbit((i + 1) * 32 - 1);
		temp.num[i] = num[i] << 1;
	}
	for (int i = 1; i < 64; i++)
	{
		temp.writebit(i * 32, highest[i - 1]);
	}
}



bool bigint::check0()//检查是否是0,是返回1
{
	for (int i = 0; i < 64; i++)
		if (num[i]) return 0;
	return 1;
}

bool bigint::equal(unsigned int x)
{
	bigint temp;
	temp.copy(*this);
	temp.num[0] -= x;
	if (temp.check0())return 1;
	else return 0;
}


void bigint::print()
{
	//十六进制
	//cout << "0x";
	int flag = 0;
	for (int i = 63; i >= 0; i--)
	{
		if (!num[i] && !flag)continue;
		if (num[i])flag = 1;
		printf("%08X", num[i]);
			cout << "";
	}
	cout << endl;
}




void bigint::pow_(bigint m, bigint e, bigint n, bigint& c)//c=m^e mod n
{
	//快速幂算法，二进制 左移
	memset(&c, 0, sizeof(c));
	//c.deg = 64;
	c.num[0] = 1;
	bigint s;
	mul(m, m, s);
	bigint buffer[1025];
	buffer[0].copy(m);
	
	for (int i = 1; i < 1025; i++)
	{
		mul(buffer[i - 1], buffer[i - 1], buffer[i]);
		if (compare(buffer[i], n) == 1)
			mod(buffer[i], n, buffer[i]);
	}

	for (int i = 32 * 32 - 1; i >= 0; i--)
	{
		if (e.getbit(i))
		{
			mul(c, buffer[i], c);
			if (compare(c, n) == 1)
				mod(c, n, c);
		}
	}

	//for (int i = 0; i <64*32; i++)
	//{
	//	bigint temp;
	//	temp.num[0] = 1;
	//	if (e.getbit(i))
	//	{
	//		for (int j = 0; j < i; j++)
	//		{
	//			mul(temp, s, temp);
	//			if (compare(temp, n)==1)//temp>n
	//				mod(temp, n, temp);
	//		}
	//		mul(c, temp, c);
	//		mod(c, n, c);
	//	}
	//}
}

void getinv(bigint a, bigint b, bigint& c)//求逆元a>b b mod a
{

}



prime1::prime1()
{
	
	//生成
	getsmallprime();
	getarbitrary();
	check2000();
	millerrabin();

}

void prime1::getarbitrary()
{
	//生成512bit随机数
	srand((unsigned)time(NULL));
	number.num[0]=rand();//随机生成x0

	number.num[0] = 5;

	for (int i = 1; i < 16; i++)
	{
		bigint a;
		a.num[0] = LINEAR_A;
		bigint m;
		m.num[0] = LINEAR_M;
		bigint temp;
		bigint temp2;
		temp2.num[0] = number.num[i - 1];
		bigint::mul(a, temp2, temp);
		bigint::mod(temp, m, temp2);
		number.num[i] = temp2.num[0];
	}
	number.num[15] |= 1<<31;
	number.num[0] |= 1;//最高位和最低为置1
}



void prime1::getsmallprime()
{
	//找到2000内的素数
	int a[2001];
	memset(a, 0, sizeof(a));
	for (int i = 2; i <= 45; i++)
	{
		for (int j = 2; i*j<=2000; j++)
		{
			a[i * j] = 1;
		}
	}
	int index = 0;
	for (int i = 2; i < 2001; i++)
	{
		if (!a[i])
			smallprime[index++] = i;
	}
}




void prime1:: check2000()//检查是否能被2000以内的素数整除
{
	int flag = 1;
	do
	{
		flag = 1;
		for (int i = 0; i < 303; i++)
		{
			bigint temp;
			temp.num[0] = smallprime[i];
			bigint r;
			//bigint::mod(number, temp, r);
			bigint::mod(number, smallprime[i], r);
			if (!r.num[0])//和数
			{
				flag = 0;
				break;
			}
		}
		if (!flag)
			number.add(2);
	} while (!flag);
	number.print();
}

void prime1::millerrabin()//检查是否是素数
{
	bigint n1;
	n1.copy(number);
	n1.sub(1);//n-1
	int flag = 0;
	while(!flag)
	{
		flag = 1;
		int h = 0;
		unsigned int a[5] = { 2,3,6,8,13 };
		for (; h < 4; h++)//5次
		{
			//srand((unsigned)time(NULL));
			//unsigned int a = rand()%(n1.num[0]-3)+2;//随机生成a
			bigint a_;
			a_.num[0] = a[h];

			bigint d;

			d.num[0] = 1;
			for (int i = 511; i >= 0; i--)
			{
				bigint x;
				x.copy(d);
				bigint temp;
				bigint::mul(d, d, temp);
				bigint::mod(temp, number, d);
				
				if (d.equal(1)&&!x.equal(1) && !bigint::compare(x, n1))//x!=1和n-1
				{
					flag = 0;
					break;
				}
				if (n1.getbit(i))
				{
					bigint temp;
					bigint::mul(d, a_, temp);
					bigint::mod(temp, number, d);
				}
			}
			if (!d.equal(1))
			{
				flag = 0;
				break;
			}
			else flag = 1;
		}
		if (flag == 0)
		{
			number.add(2);
			check2000();
			h = 0;
		}
	}
}


//密钥生成
RSA::RSA(bigint p, bigint q)
{
	this->p.copy(p);
	this->q.copy(q);
	bigint::mul(this->p, this->q, this->n);
	bigint temp1, temp2;
	temp1.copy(this->p);
	temp2.copy(this->q);
	bigint t;
	t.num[0] = 1;
	bigint::sub(p, t, temp1);
	bigint::sub(q, t, temp2);
	//phi.deg = 32;
	//n.deg = 32;
	bigint::mul(p, q, n);
	bigint::mul(temp1, temp2, phi);
	while (1)
	{
		srand((unsigned)time(NULL));
		//e.deg = 32;
		e.num[0] = (2 * rand() + 1) + 1;//随机生成e,奇数互素的概率更高
		bigint temp;
		bigint::mul(phi, e, temp);
		if (!temp.check0())
		{
			break;
		}
	}

	//d.deg = 32;
	bigint::getinv(phi, e, d);
}



RSAen::RSAen(bigint n, bigint e, bigint m)
{
	this->m.copy(m);
	bigint::pow_(this->m, this->e, this->n, this->c);
	this->c.print();
}

RSAde::RSAde(bigint n, bigint d, bigint c)
{
	this->c.copy(c);
	bigint::pow_(this->c, this->d, this->n, this->m);
	this->m.print();
}



void bigint:: getinv(bigint a, bigint b, bigint& c)//求逆元a>b b mod a

{

}
