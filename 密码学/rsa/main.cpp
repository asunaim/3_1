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
	big e;
	e.set(0x10001);

	string a_ = "C8EA3821A75DF1DD0A1B28B975655DFB";
	string b = "E2CD73254B1F1FF761F79DCFC4E92CFB";

	prime p(a_), q(b);

	RSA_ a(p.number, q.number,e);
	big m;
	m.set("Hi",0);
	RSAen_ temp(a, m);
	temp.c.print();

	RSAde_ temp_(a, temp.c);
	temp_.m.print();
}

prime::prime(string a)
{
	number.set(a);
}
prime::prime(string a,int x)
{
	number.set(a,x);
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

	//num[0] = 5;

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
				temp.set(a[k]);
				d.set(1);
				for (int i = high * 4 + 3; i >= 0; i--)
				{
					cout << i << endl;
					big x;
					x.copy(d);
					d = mul(d, d);
					d = mod(d, number);
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
			}
		}
	}
	cout << "素数: "; number.print();
}




RSA_::RSA_(big p, big q,big e)
{
	big v1;
	v1.set(1);

	this->p = p;
	this->q = q;
	this->e = e;

	n = mul(p, q);
	big p1, q1;
	p1 = sub(p, v1);
	q1 = sub(q, v1);

	phi = mul(p1, q1);
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


int change(char a)
{
	if (a >= '0' && a <= '9')
		return (int)(a - 48);
	switch (a)
	{
	case 'a':
	case 'A':
		return 10;
	case 'b':
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


void big::set(string a)//16进制输入
{
	//没有0x
	int len = a.length();
	for (int i = len - 1; i >= 0; i--)
		num[len - i - 1] = change(a[i]);
}

void big::set(string a,int x)//字符串输入
{
	//没有0x
	int len = a.length();
	for (int i = len - 1; i >= 0; i--)
	{
		num[2*len - 2*i - 2] = (int)a[i]%16;
		num[2 * len - 2 * i - 1] = (int)a[i] / 16;
	}
}


