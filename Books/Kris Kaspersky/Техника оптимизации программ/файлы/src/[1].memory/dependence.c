/* ---------------------------------------------------------------------------
 * @
 *				������� ��� ��������� ���������� �����������
 *		����������� ������ ��� ������ ��������� � ����������� ������
 *		============================================================
 *
 * Build 0x003	12.06.2002
 * Build 0x002	16.05.2002
--------------------------------------------------------------------------- */

// ������������
// ============
#define BLOCK_SIZE (16*M)					// ������ ��������������� �����


#include <DoCPU.h>

main()
{
	int a;
	int x=0;
	int *p1, *p2;

	// ��������� �������������� ������ �������
	p1 = malloc(BLOCK_SIZE);
	p2 = malloc(BLOCK_SIZE);
	
	// TITLE
	PRINT("=== ������������ ������������� ��������� ����������� ������ ===\n");
	PRINT_TITLE;
	PRINT("------------------ ������������ ------------------\n");

	/* -----------------------------------------------------------------------
	 *
	 *		��������� ���������� ����������� ��� ������ ��������� ������
	 *
	----------------------------------------------------------------------- */

	// ��������� ������ ������, �.�. ��� ���������� ������������ ���������
	// ������ ��� ��������� ���������.
	memset(p1, 0, BLOCK_SIZE);
	CLEAR_L2_CACHE();

	A_BEGIN(0)
		// ������������� ���� ��� ������������ ����������� CPU
		for (a=0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			// ������ ������
			x = *(int *)((char *)p1 + a + 0 * sizeof(int));

			// ����� ��������� ������ ����������� �� ������ �������� ����������
			// �������, ��������� �� ����� �������� ������ ������� �� ��� ���,
			// ���� �� ������� ��� ������ � ���� ������������
			a += x;

			// ������ - ����������...
			x = *(int *)((char *)p1 + a + 1 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 2 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 3 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 4 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 5 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 6 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 7 * sizeof(int));
			a += x;
		}
	A_END(0)
	printf("%s:\t %4.2f (Mbytes/s)\n",		/* ����� ��������� �� ����� */
	_TEXT("������ ���������   ������"), ((BLOCK_SIZE)/cpu2time(Ax_GET(0))));

	/* -----------------------------------------------------------------------
	 *
	 *	��������� ���������� ����������� ��� ������ _��_��������� ������
	 *
	----------------------------------------------------------------------- */
	CLEAR_L2_CACHE();

	A_BEGIN(1)
		for (a=0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			// ������ ��������� ����� �������� ��������� ������ �� ������
			// �� ��������� ���������� �����������, ��������� ��� �����
			// ����� �� ������ � ��������������� �������
			x += *(int *)((char *)p2 + a + 0 * sizeof(int));
			x += *(int *)((char *)p2 + a + 1 * sizeof(int));
			x += *(int *)((char *)p2 + a + 2 * sizeof(int));
			x += *(int *)((char *)p2 + a + 3 * sizeof(int));
			x += *(int *)((char *)p2 + a + 4 * sizeof(int));
			x += *(int *)((char *)p2 + a + 5 * sizeof(int));
			x += *(int *)((char *)p2 + a + 6 * sizeof(int));
			x += *(int *)((char *)p2 + a + 7 * sizeof(int));
		}
	A_END(1)

	// ����� ����������� �� �������
	printf("%s:\t %4.2f (Mbytes/s)\n",		/* ����� ��������� �� ����� */
	_TEXT("������ ����������� ������"),((BLOCK_SIZE)/cpu2time(Ax_GET(1))));


	PRINT(_TEXT("--------------------------------------------------\n"));
return x;

}