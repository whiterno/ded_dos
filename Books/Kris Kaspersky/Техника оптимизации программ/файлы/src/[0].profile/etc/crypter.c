/*-----------------------------------------------------------------------------
 * [���� �� �� ������ �������� ���� �����, �� ������� ������������ ���������]
 * 
 *   						��������� ��� ������� PSWD
 *							--------------------------
 *
 *		������ �ணࠬ�� ��஢��  ��������  ���  ᮧ������  ᢮�  ᮡ�⢥���
 * ����஢���� 䠩��,�।�����祭�� ��� ������᪮� ����஢�� �ணࠬ���
 * pswd.exe
 *
 * Build 0x001 12.08.2002
----------------------------------------------------------------------------*/
#include <memory.h>
#include <stdio.h>

// ����. ����� ������� ����
#define MAX_STR_LEN	102

// ࠧ����⥫� �����
#define SEPARAT		(":")

// ����� ��ப�
#define EOLs		("\n")

/*----------------------------------------------------------------------------
 *
 *				��楤�� ��஢�� ���⥪�� �������� ��஫��
 *				-------------------------------------------------
 *	ARG:
 *		pswd		:	��஫�
 *		data		:	⥪�� ��� ��஢��
 *
 *	RET:
 *		data		:	����஢���� �⥪��
 *
 *	NOTE:			0	- ���ࠢ���� ��஫� (XOR � ���. ⥪�. ���� 0)
 *					1	- ALL OK
 *		
-----------------------------------------------------------------------------*/
int Crypt(char *pswd, char *data)
{
	int a;
	int p = 0;		// 㪠��⥫� ⥪�饩 ����樨 ���㥬�� ������

	// * * * �������� ���� �������� * * *
	do {
		// ���㥬 ⥪�騩 ᨬ���
		data[p] ^= pswd[p % strlen(pswd)];
		if (!(data[p])) return 0;

		// ���室�� � ��஢�� ᫥���饣� ᨬ����
	} while(++p < strlen(data));
	return 1;
}


/*----------------------------------------------------------------------------
 *
 *				��楤�� ���᫥��� ����஫쭮� �㬬� ��஫�
 *				---------------------------------------------
 *	ARG:
 *		pswd		:	��஫�
 *
 *	RET:			:	CRC ������� ��஫�
 *
 *	NOTE:
 *		none
-----------------------------------------------------------------------------*/
int CalculateCRC(char *pswd)
{
	int a;
	int x = -1;			// �訡�� ���᫥��� CRC

	for (a = 0; a < strlen(pswd);  a++)
	{
		x += *(int *) ((char *)pswd + a);
	}

	return x;
}

main()
{
	int		a;
	int 	x;
	int 	len;
	int 	CRC;
	char 	rem[MAX_STR_LEN];
	char 	dat[MAX_STR_LEN];
	char 	psw[MAX_STR_LEN];

	memset(psw, 0, 	MAX_STR_LEN);
	memset(dat, 0, 	MAX_STR_LEN);

	// ���� ������ � ���᮫�
	fputs("KPNC INTERNAL USAGE\n",stderr);
	fputs("= = = ��஢�� ��ப ��� �ਬ�� ��䨫�஢�� pswd = = =\n",stderr);
	fputs("������ ⥪�� ���������:",stderr);fgets(rem,100,stdin); rem[strlen(rem)-1]=0;
    fputs("������  ���㥬�  ⥪��:",stderr);fgets(dat,100,stdin); dat[strlen(dat)-1]=0;
    fputs("������  ��஫�  ��஢��:",stderr);fgets(psw,100,stdin); psw[strlen(psw)-1]=0;

    if ((strlen(rem)==0) || (strlen(dat)==0) || (strlen(psw)==0))
    {
    	fputs("-ERR: ���ࠢ���� ����! �ய�饭 ���� ����� ��� ����� ��ப!\n",stderr);
    	return 0;
    }

    // ���᫥��� ����஫쭮� �㬬� ��஫�
    CRC = CalculateCRC(psw);

    // ��஢�� ��஫�
    if (!Crypt(psw, dat)) 
    {
    	fputs("-ERR: ���ࠢ���� ��஫� - XOR � ���⥪�⮬ ���� ����\n",stderr);
    	return 0;
    }

    // �뢮� १���⮢ �� ���᮫�	
    fwrite(rem,		1,				strlen(rem),	stdout);
    fwrite(SEPARAT,	1, 				strlen SEPARAT, stdout);
    fwrite(&CRC,	sizeof(CRC),	1,  			stdout);
    fwrite(dat, 	1, 				strlen(dat)+1,	stdout);
    fwrite(EOLs, 	1, 				strlen EOLs,	stdout);


}
	

