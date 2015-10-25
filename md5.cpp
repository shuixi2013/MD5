/* 
* @Author: JinJin Lin
* @Date:   2015-10-25 12:54:30
* @Last Modified by:   linjinjin
* @Last Modified time: 2015-10-25 20:14:49
*/
#ifndef MY_MD5
#define MY_MD5

#include <iostream>
#include <string>
#include <memory.h>
using namespace std;
#define shift(x, n) (((x) << (n)) | ((x) >> (32-(n))))//右移的时候，高位一定要补零，而不是补充符号位
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))    
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

string changeIntToHex(int orginNum) {
	const char str16[]="0123456789abcdef";
    int hexNum;
    string temp;
    string hexString="";
    for(int i = 0; i < 4; i++) {
    	//处理每个字节，一个字节8bit，一个int32bit，所以循环4次
        temp="";
        //(1<<8) == 256, 取256的模，就是取低8位， &0xff 把高于8位的地方都置0，最终得到低8位的值
        hexNum = ((orginNum >> i*8 ) % (1<<8)) & 0xff;   //逆序处理每个字节
        for (int j = 0; j < 2; j++) {
            temp.insert(0, 1, str16[hexNum%16]);
            hexNum = hexNum/16;
        }
        hexString += temp;
    }
    return hexString;
}

string md5(string message){
	const unsigned s[64] = { 	
		7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
		5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
		4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
		6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 
	};
    
	const unsigned k[64] = {	
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,				
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

	unsigned int A = 0x67452301;   //A
	unsigned int B = 0xefcdab89;   //B
	unsigned int C = 0x98badcfe;   //C
	unsigned int D = 0x10325476;   //D		

	int messLength = message.length();
	unsigned int block64byte_Num = (( message.length() + 8)/64) + 1; 
	// +8是加了64bit用来放置message长度信息的预留位置，+1是因为填充总是要进行，即使message+message.length能整除512bit
	unsigned int *messageByte = new unsigned int[block64byte_Num * 16];
	//因为一个int类型有32bit，正好是我们把一个512bit的块划分为16个子块后的大小
	//那么我们有block64byte_Num个512块，就有block64byte_Num*16个子块
	memset(messageByte, 0, sizeof(messageByte)*block64byte_Num*16);
	// for (unsigned int i = 0; i < block64byte_Num*16; i++) {
	// 	messageByte[i] = 0;
	// }
	for (unsigned int i = 0; i < messLength; i++) {
		messageByte[i>>2] |= (message[i]) << ((i%4)*8);
		//一个整数存储四个字节，i>>2表示i/4 一个unsigned int对应4个字节，保存4个字符信息
		//messaged的0~3个字符都会存储在messageByte[0]，因为0>>2, 1>>2, 2>>2, 3>>2都等于0
	}
	messageByte[messLength >> 2] |= 0x80 << ((messLength%4)*8);
	//尾部添加1 模4是因为一个unsigned int保存4个字符信息,
	//模出来的余*8就得到了messageByte最后一个int占用了多少bit, 那么0x80就左移多少bit，
	//这里涉及到小端原则，int是低位存在高位的，也就是说，一个int中[[8bit(低位)][8bit][8bit][8bit(高位)]]，
	//那么我们要存低位的8bit,则应该左移到最左边才是低位

	messageByte[block64byte_Num*16-2] = messLength * 8;
	//添加原长度，长度指位的长度，所以要乘8，然后是小端序，所以放在倒数第二个,这里长度只用了32位，超过32位就...

	for (unsigned int i = 0; i < block64byte_Num; i++) {
		//对每一个64byte(512bit)的block进行处理
		unsigned int a = A;
		unsigned int b = B;
		unsigned int c = C;
		unsigned int d = D;
		unsigned int f, g;
		for (unsigned int j = 0; j < 64; j++) {
			if (j < 16) {
				f = F(b, c, d);
				g = j;
			} else if (j < 32) {
				f = G(b, c, d);
				g = (5*j+1) % 16;
			} else if (j < 48) {
				f = H(b, c, d);
				g = (3*j+5) % 16; 
			} else {
				f = I(b, c, d);
				g = (7*j)%16;
			}
			unsigned tempd = d;
			d = c;
			c = b;
			b = b + shift(a + f + k[j] + messageByte[i*16+g], s[j]); 
			//messageByte[i*16+j]是消息中第i个512bit块的第j个32位字
			a = tempd;
		}
		A = a + A;
		B = b + B;
		C = c + C;
		D = d + D;
	}

	return changeIntToHex(A)+changeIntToHex(B)+changeIntToHex(C)+changeIntToHex(D);
}

#endif