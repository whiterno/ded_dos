/* ---------------------------------------------------------------------------
 *
 *						������� ��������������� �������
 *		������������ ������� �� �������� ��������� ������� ������� ������
 *		=================================================================
 *
 * Build 0x001 06.07.2002
--------------------------------------------------------------------------- */
// ������������
#define BLOCK_SIZE		(8*M)			// ������ ��������������� �����

#include <DoCPU.h>
main()
{
	int *p1;
	int a, x = 0;

	// TITLE
	PRINT("= = = ������� ������������ ������� �� �������� ��������� ������ ������ = = =\n");
	PRINT_TITLE;

	// �������� ������
	p1 = malloc(BLOCK_SIZE);


	/*------------------------------------------------------------------------
	 *
	 *						��� ��������� �����
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		// ���. ������
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			x+= *(int *) ((char *)p1 + a);
	A_END(0)

	VVV;
	A_BEGIN(1)
		// �� ���. ������
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			x+= *(int *) ((char *)p1 + a + 1 /* <- ��������� */);
		A_END(1)

	VVV;
	A_BEGIN(2)
		// ���. write
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			*(int *) ((char *)p1 + a) = x;
	A_END(2)

	VVV;
	A_BEGIN(3)
		// �� ���. write
		for (a = 0; a < BLOCK_SIZE; a += sizeof(int))
			*(int *) ((char *)p1 + a + 1) = x;
	A_END(3)

	/*------------------------------------------------------------------------
	 *
	 *					� ���������� ����� �� 8 ��������
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(4)
		// ���. ������
		for (a = 0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			x += *(int *) ((char *)p1 + a + 0 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 1 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 2 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 3 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 4 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 5 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 6 * sizeof(int));
			x += *(int *) ((char *)p1 + a + 7 * sizeof(int));
		}
	A_END(4)


	VVV;
	A_BEGIN(5)
		// �� ���. ������
		for (a = 0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			x += *(int *) ((char *)p1 + a + 0 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 1 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 2 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 3 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 4 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 5 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 6 * sizeof(int) + 1);
			x += *(int *) ((char *)p1 + a + 7 * sizeof(int) + 1);
		}
	A_END(5)


	VVV;
	A_BEGIN(6)
		// ���. write
		for (a = 0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			*(int *) ((char *)p1 + a + 0 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 1 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 2 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 3 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 4 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 5 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 6 * sizeof(int)) = x;
			*(int *) ((char *)p1 + a + 7 * sizeof(int)) = x;
		}
	A_END(6)


	VVV;
	A_BEGIN(7)
		// �� ���. write
		for (a = 0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			*(int *) ((char *)p1 + a + 0 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 1 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 2 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 3 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 4 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 5 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 6 * sizeof(int) + 1) = x;
			*(int *) ((char *)p1 + a + 7 * sizeof(int) + 1) = x;
		}
	A_END(7)
	
	// ����� ����������� �� �������
	Lx_OUT("unalign read  without unroll",Ax_GET(0),Ax_GET(1));
	Lx_OUT("unalign write without unroll",Ax_GET(2),Ax_GET(3));
	Lx_OUT("unalign read  with    unroll",Ax_GET(4),Ax_GET(5));
	Lx_OUT("unalign write with    unroll",Ax_GET(6),Ax_GET(7));
}

_P_S()
{
/*
	"���� ����, ������� ����� ������� -- �� ������ ������. �� ����� ���������,
	�������� � ���� � �����. ��-�� ������� ����� ������ ���� ��� �� ����� ����
	� ����. ��� ����������. ��� � ������ ������ ��� ������ ���� ������� �����.
	� ���� �� ������ �������� ������� ����, ����� ����� � ��� ����� ���������,
	���������  �����,  ��  ����  ��������� ��� ��". ��-�� ������� � ����������
	����  �������  �����  �����  �����  ����������.  ���  ������ �� ����� ����
	�������  �������  �����,  ����������  ����������:  ��  ��������,  �������,
	���������,  �������,  ��������� ������ ����, �� ���� ����� ��� ��. �� ����
	�����  ���  �������  ����������  � ���������� ������� ����, �� ��� ����� �
	���."
														�-�� "������ �����"
*/
}