/* ---------------------------------------------------------------------------
 * @
 *							������� ���������������
 *
 *						� � � � � � � � � � � � � � �
 *
 *					������������ �������� ������ ���� int
 *
 * Build 0x001 05.07.2002
--------------------------------------------------------------------------- */
// ������������
#define BLOCK_SIZE	(8*M)

// ����������� ����� ��������
#define N			(BLOCK_SIZE/sizeof(int))


#include <DoCPU.h>

// -[������ ����� �������]----------------------------------------------------
//
//	ARG:
//		array	- ��������� �� ������
//		n		- ���-�� ��������� ��� ����������
//
// README:
//		������� �� ����������� ��������� �� ������! ������� ��� ����!
//----------------------------------------------------------------------------
int sum(int *array, int n)
{
	int a, x = 0;
	for(a = 0; a < n; a += 8)
	{
		x+=array[a + 0];
		x+=array[a + 1];
		x+=array[a + 2];
		x+=array[a + 3];
		x+=array[a + 4];
		x+=array[a + 5];
		x+=array[a + 6];
		x+=array[a + 7];
	}
	return x;
}

// -[������ ����� �������]----------------------------------------------------
//
//	ARG:
//		array	- ��������� �� ������
//		n		- ���-�� ��������� ��� ����������
//
// README:
//		������� ����������� � �������������� ���������, � ���� ����������� ��
//	(������, ���� �� �����, ���� �� ��� ��� �� ������)
//----------------------------------------------------------------------------
int sum_align(int *array, int n)
{
	int a, x = 0;
	char supra_bytes[4];
//	supra_bytes=_malloc32(4);

		// ��������: ��� ������� ��� _��������_ ������
		// ����� array & 15== 1, �.�. �������� ������,
		// �������� ������ �� 1 ���� ������ ������������
		// ������������ �� ������� 32 ���� ������
		// ����� ������� ������ ������ ��� �������������
		// ������ (���� - ������� ������������������)
		// ����������!
		// ������������ ������� - ������� ������� ����
		// "����������" ��� ������ ��������
		// ���� �� ����� 32 - 32/4 = 24, ��� �������
		// ��������� ��� �������� ��������
	if (((int)array & 15)!=1)
		ERROR("-ERR: ������������ ������������\n");
		
	for(a = 0; a < n; a += 8)
	{
		
		// �������� ��� ������� �����, �������
		// �� ���������� ������� �������� ������
		// ������
		x+=array[a + 0];
		x+=array[a + 1];
		x+=array[a + 2];
		x+=array[a + 3];
		x+=array[a + 4];
		x+=array[a + 5];
		x+=array[a + 6];

		// ������� �����, ������������ �������� ����
		// �������� �� ��������� ����� �� � � � � � �
		supra_bytes[0]=*((char *) array + (a+7)*sizeof(int) + 0);
		supra_bytes[1]=*((char *) array + (a+7)*sizeof(int) + 1);
		supra_bytes[2]=*((char *) array + (a+7)*sizeof(int) + 2);
		supra_bytes[3]=*((char *) array + (a+7)*sizeof(int) + 3);

		// ��������� sura-����� � ������������ �� ��� ������� �����
		x+=*(int *)supra_bytes;
	}

	return x;
}


main()
{
	int a;
	int *p;

	PRINT("= = = ������������ � � � � � � � � � � � � � � �  ������������ = = =\n");
	PRINT_TITLE;

	// �������� ������
	p = (int *) _malloc32(BLOCK_SIZE);

	/*------------------------------------------------------------------------
	 *
	 *					������������ ����������� ����
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		sum(p, N);
	A_END(0)

	/*------------------------------------------------------------------------
	 *
	 *					������������ ������������� ����
	 *
	----------------------------------------------------------------------- */
	// "���������" ���������
	p = (int *)((int)p + 1);

	VVV;
	A_BEGIN(1)
		sum(p, N);
	A_END(1)

	/*------------------------------------------------------------------------
	 *
	 *				�������� ��������� ���� �� ���������� �������
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
		sum_align(p, N);
	A_END(2)

	// ����� ����������� �� �������
	Lx_OUT("unalign.....",Ax_GET(0),Ax_GET(1));
	Lx_OUT("try optimize",Ax_GET(0),Ax_GET(2));
	
	
}

_P_S()
{
/*
	�  ���������  �  ����������  �����,  ������������ �������, ������� �������
	������������    ������,   ���������   ���������   �   �������.
											������  �� ����-��������. ��������
*/
}



