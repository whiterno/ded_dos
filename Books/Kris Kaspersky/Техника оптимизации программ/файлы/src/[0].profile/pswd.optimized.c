/*----------------------------------------------------------------------------
 *
 *				������, ��������������� ������� �����������
 *			� ������������ � ������� �������������� Intel VTune
 *			====================================================
 *
 * Build 0x001 12.08.2002
----------------------------------------------------------------------------*/
// STEP 1
//			����� printf ��� ����������� �������� ���� ����� 6666 ��������

// STEP 2
//			������� strlen �� ���� ����� � ������� CalculateCRC

//STEP 3
//			������������ pswd

// STEP 4
//			���������� ����� ������ gen_pswd

// STEP 5
//			������� ���������� ��������� �� gen_pswd

// STEP 6
//			������� ����������� �������� ���������

// STEP 7
//			�������� �������

// ������������
#define ITER 40000000						// ����. ��������
#define MAX_CRYPT_LEN	200					// ����. ����� �����������

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>

int length;
/*----------------------------------------------------------------------------
 *
 *				��������� ����������� ����������� ��������� �������
 *				---------------------------------------------------
 *	ARG:
 *		pswd		:	������
 *		crypteddata	:	����������
 *
 *	RET:
 *		*crypreddata:	�������������� ���������
 *
 *	NOTE:
 *		none
-----------------------------------------------------------------------------*/
DeCrypt(char *pswd, char *crypteddata)
{

	unsigned int p = 0;		// ��������� ������� ������� ���������������� ������

	// * * * �������� ���� ����������� * * *
	do {
		// �������������� ������� ������
		crypteddata[p] ^= pswd[p % strlen(pswd)];
		// ��������: ������ ������� ^^^ _�����_ ��������� ��������
		// ��� �� ����� ����� �� ����� ������������  ������������,
		// �.�. ��� ������� ����������  ������ ����� (����  ������
		// ����������

		// ��������� � ����������� ���������� �������
	} while(++p < strlen(crypteddata));
}

int gen_pswd(char *crypteddata, char *pswd, int max_iter, int validCRC)
{
	int a, b, x;
	int p = 0;
	char *buff;
	int y=0;
	int k;
	int length = strlen(pswd);
	
	x = -1;
	for (b = 0; b <= length;  b++)
		x += *(int *)((int)pswd + b);

	// ������������ ������
	for(a = 0; a <  max_iter ; a++)
	{

		if (x==validCRC)
		{
		// �������� ����������� �� ��������� �����
		buff = (char *) malloc(strlen(crypteddata));
		strcpy(buff, crypteddata);

		// ��������������
		DeCrypt(pswd, buff);
	
			// ������� ��������� ����������� �� �����
			printf("CRC %8X: try to decrypt: \"%s\"\n", validCRC,buff);
		}
		y = 1;
		k = 0x59;
		while((++pswd[p])>'z')
		{
			pswd[p] = '!';
			// ��������� ������
			y = y | y << 8;
			x -= k;
			k = k << 8;
			k += 0x59;
			
			p++;
			if (!pswd[p])
			{
					pswd[p]='!';
					pswd[p+1]=0;
					length++;
						x = -1;
						for (b = 0; b <= length;  b++)
						x += *(int *)((int)pswd + b);
					pswd[p]=' ';
						y = 0;
			}
			//printf("%x\n",y);
		} // end while(pswd)
		p = 0;
		x+=y;
	} // end for(a)

	return 0;
}

/*----------------------------------------------------------------------------
 *
 *				�������� ������� �����, �������� ������� �������
 *				------------------------------------------------
 *	ARG:
 *		per			:	����� ��� ������
 *
 *	RET:
 *		none
 *
 *	NOTE:
 *		������� ������� �� ����� �����, ������ ��� �� ����� �����
 *
-----------------------------------------------------------------------------*/
print_dot(float per)
{
	// * * * ������������ * * *
	#define N			3		// �������� �� ��� �������
								// ��� ������ HEX ���� ������� ���

	#define DOT_SIZE	1		// ������ �����-�����������

	#define	DOT			"."		// �����������
	
	int		a;
	char	buff[666];

	sprintf(buff,"%0.0f", per);
	/* ^^^^^^^^^^^^^^^^ ������ ������ */

	// * * * ���� ������ ����� �� �������� * * *
	for(a = strlen(buff) - N; a > 0; a -= N)		// <-- �������
	{ /* ^^^^^^^^^^^^^^^^ - ��� ������ ���, - �� ��������� ������� ����� */

			memmove(buff + a + DOT_SIZE, buff + a, 66);
			/* ��������!						^^^^^^^^^ */

			if(buff[a]==' ') break;	// ��������� ������ - ����� ������
				else
			// �������� �����������
			memcpy(buff + a, DOT, DOT_SIZE);
	}
	// �������� �� �����
	printf("%s\n",buff);
}


main(int argc, char **argv)
{
	// ����������
	FILE *f;				// ��� ������ ��������� ����� (���� ����)
	char *buff;				// ��� ������ ������ ��������� �����
	char *pswd;				// ������� ����������� ������ (need by gen_pswd)
	int validCRC;			// ��� �������� ������������� CRC ������
	unsigned int t;			// ��� ������ ������� ���������� ��������
	int iter = ITER;		// ����. ���-�� ������������ �������
	char *crypteddata;		// ��� �������� �����������

	//	build-in default crypt
	//	��� �������, ��� �����  �����������, ���  ��������  �������  �����
	//	���� ��������� ;)
	char _DATA_[] = "\x4B\x72\x69\x73\x20\x4B\x61\x73\x70\x65\x72\x73\x6B"\
					"\x79\x20\x44\x65\x6D\x6F\x20\x43\x72\x79\x70\x74\x3A"\
					"\xB9\x50\xE7\x73\x20\x39\x3D\x30\x4B\x42\x53\x3E\x22"\
					"\x27\x32\x53\x56\x49\x3F\x3C\x3D\x2C\x73\x73\x0D\x0A";

	// TITLE
	printf("= = = VTune profiling demo = = =\n==================================\n");

	// HELP
	if (argc==2)
	{
			printf("USAGE:\n\tpswd.exe [StartPassword MAX_ITER]\n");
			return 0;
	}
	
	// ��������� ������
	printf("memory malloc\t\t");
	buff = (char *) malloc(MAX_CRYPT_LEN);
	if (buff) printf("+OK\n"); else {printf("-ERR\n"); return -1;}

	// ��������� ����������� ��� �����������
	printf("get source from\t\t");
	if ((f=fopen("crypted.dat","r"))!=0)
	{
		printf("crypted.dat\n");
		fgets(buff,MAX_CRYPT_LEN, f);
	}
	else
	{
		printf("build-in data\n");
		buff=_DATA_;
	}

	// ��������� CRC
	validCRC=*(int *)((int) strstr(buff,":")+1);
	printf("calculate CRC\t\t%X\n",validCRC);
	if (!validCRC)
	{
		printf("-ERR: CRC is invalid\n");
		return -1;
	}

	// ��������� ����������� ������
	crypteddata=strstr(buff,":") + 5;
	//printf("cryptodata\t\t%s\n",crypteddata);

	// ��������� ������ ��� ���������� ������ 
	printf("memory malloc\t\t");
	pswd = (char *) malloc(512*1024);
	//pswd+=62;

	memset(pswd,0,666);		// <-- ������������ 

	if (pswd) printf("+OK\n"); else {printf("-ERR\n"); return -1;}
	
	// ������ ���������� ��������� ������
	// ��������� ���������� ������ � ����. ���-�� ��������
	printf("get arg from\t\t");
	if (argc>2)
	{
		printf("command line\n");
		if(atol(argv[2])>0) iter=atol(argv[2]);
		strcpy(pswd,argv[1]);
	}
		else
	{
		printf("build-in default\n");
		strcpy(pswd,"!");
	}
	printf("start password\t\t%s\nmax iter\t\t%d\n",pswd,iter);
	

	// ������ �������� �������
	printf("==================================\ntry search... wait!\n");
	t=clock();
		gen_pswd(crypteddata,pswd,iter,validCRC);
	t=clock()-t;

	// ����� ���-�� ������������ ������� �� ���
	printf("                                       \rPassword per sec:\t");
	print_dot(iter/(float)t*CLOCKS_PER_SEC);
	//printf("%d\n",t);

	return 0;
}