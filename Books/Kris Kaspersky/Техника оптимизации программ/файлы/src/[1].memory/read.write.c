/* ---------------------------------------------------------------------------
 * @
 *							������� ��� ������
 *		��������� �� ������������������ ���������� ��������� ������/������
 *		==================================================================
 *
 * Build 0x001 30.06.2002
--------------------------------------------------------------------------- */

// ������������
#define BLOCK_SIZE	(8*M)

#include <DoCPU.h>
main()
{
	int a;
	int x=0, y=0;
	int *p1, *p2;

	// TITLE
	PRINT("= = = ��������� �� ������������������ ��������� ��������� ������/������ = = =\n");
	PRINT_TITLE;

	// ��������� ������
	p1 = malloc(BLOCK_SIZE);
	p2 = malloc(BLOCK_SIZE);

	/* -----------------------------------------------------------------------
	 *
	 *			���������� �� ������ � ������ _��_ �������������
	 *				������� ��������� ����� �� ������
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		// ������
		for (a = 0; a < BLOCK_SIZE; a += 4)
				x += *(int *)((char *)p1 + a);

		// ������
		for (a = 4; a < BLOCK_SIZE; a += 4)
			*(int *)((char *)p2 + a) = y;
	A_END(0)


	/* -----------------------------------------------------------------------
	 *
	 *			���������� �� ������ � ������ _�������������_
	 *				������� ��������� ����� �� ������
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(1)
		for (a = 0; a < BLOCK_SIZE; a += 4)
		{
			// ������
			x += *(int *)((char *)p1 + a);

			// ������
			*(int *)((char *)p2 + a + 4) = y;
		}
	A_END(1)

	/* -----------------------------------------------------------------------
	 *
	 *			���������� �� ������ � ������ _��_ �������������
	 *					������� ��������� ����� ������
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
		// ������
		for (a = 0; a < BLOCK_SIZE; a += 32)
		{
			x += *(int *)((char *)p1 + a);
			x += *(int *)((char *)p1 + a + 4);
			x += *(int *)((char *)p1 + a + 8);
			x += *(int *)((char *)p1 + a + 12);
			x += *(int *)((char *)p1 + a + 16);
			x += *(int *)((char *)p1 + a + 20);
			x += *(int *)((char *)p1 + a + 24);
			x += *(int *)((char *)p1 + a + 28);
		}

		// ������
		for (a = 4; a < BLOCK_SIZE; a += 32)
		{
			*(int *)((char *)p2 + a) = y;
			*(int *)((char *)p2 + a + 4) = y;
			*(int *)((char *)p2 + a + 8) = y;
			*(int *)((char *)p2 + a + 12) = y;
			*(int *)((char *)p2 + a + 16) = y;
			*(int *)((char *)p2 + a + 20) = y;
			*(int *)((char *)p2 + a + 24) = y;
			*(int *)((char *)p2 + a + 28) = y;
		}
	A_END(2)

	/* -----------------------------------------------------------------------

				���������� �� ������ � ������ _�������������_
						������� ��������� ����� ������

	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(3)
		for (a = 0; a < BLOCK_SIZE; a += 16)
		{
			// ������
			x += *(int *)((char *)p1 + a);
			x += *(int *)((char *)p1 + a + 4);
			x += *(int *)((char *)p1 + a + 8);
			x += *(int *)((char *)p1 + a + 12);

			// ������
			*(int *)((char *)p2 + a) = y;
			*(int *)((char *)p2 + a + 4) = y;
			*(int *)((char *)p2 + a + 8) = y;
			*(int *)((char *)p2 + a + 12) = y;
		}
	A_END(3)

	// ����� ����������� �� �������
	Lx_OUT("��� ��������� �����",Ax_GET(0),Ax_GET(1));
	Lx_OUT("� ����������  �����",Ax_GET(2),Ax_GET(3));
}

_P_S()
{
/*
	"...�����    ������   �������  ��������,  �������  ���������,  ����������,
	�������,  ��������   �  ������  �������  ������������,  ��������  ��������
	�������,  ����������,   ���������  ������, �,������ ���� ����������, �����
	��������� �������������"
										������ �� ����-��������. ��������
*/
}