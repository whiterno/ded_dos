/*----------------------------------------------------------------------------
 *
 *					������������ ������������������ �������
 *				�	��������	�����	������		�	�������
 *				================================================
 *
 * Build 0x001 25.07.2002
 ---------------------------------------------------------------------------*/

// NOTE �1	��������������� ������� ������� ����� �������� ������ ��� ������
//========	�������� ������.

// NOTE �2	��� ��������������� � �� �������� ������ � �� L2 ���
// ========

// ������������
#define BLOCK_SIZE	(4*M)					// ������ ��������������� ����� ������
#define BRST_LEN	32						// ���-�� ���� � �������� ����� ������

#include <DoCPU.h>

main()
{
	int		a;
	int		*p;
	int		x =0, y = 0;

	// TITLE
	PRINT("=== ������������ ������������������ ������� � �������� ����� ������ � ������� ===\n");
	PRINT_TITLE;

	// �������� � ������������� ������
	p = malloc(BLOCK_SIZE); memset(p, 0, BLOCK_SIZE);

	/*------------------------------------------------------------------------
	 *
	 *					���������� ������� ��������
	 *
	 -----------------------------------------------------------------------*/
	A_BEGIN(0)
		for (a = 0; a < (BLOCK_SIZE - BRST_LEN); a += BRST_LEN)
		{
			x += *(int *)((char *)p + a + 0x0 + x);
			x += *(int *)((char *)p + a + 0x8 + x);
		}
	A_END(0)


	/*------------------------------------------------------------------------
	 *
	 *						��������� � �������� ������
	 *
	 -----------------------------------------------------------------------*/
	A_BEGIN(1)
		for (a = 0; a < (BLOCK_SIZE - BRST_LEN); a += BRST_LEN)
		{
			x += *(int *)((char *)p + a + 0x0  + x);
			x += *(int *)((char *)p + a + 0x18 + x);
		}
	A_END(1)


	/*------------------------------------------------------------------------
	 *
	 *	�������� ������ 0x8 & 0x10 �� ���������� � ������ � ����� �����
	 *
	 -----------------------------------------------------------------------*/
	A_BEGIN(2)
		for (a = 0; a < (BLOCK_SIZE - BRST_LEN); a += BRST_LEN)
		{
			// ======= ����������� ������� ������� �� ������������� =========
			// ���� �� ������ ������ ����� � ������ 0x0, �� ������ 0x8 � 0x10
			// ����������� � ������� �������, � ������ ��� ����� �����!
			// *
			//y += *(int *)((int)p + a + 0x0 + 0x0 + 0); // 31 % vs 75
			y += *(int *)((int)p + a + 0x0 + 0x0 + y); // 88% vs 66%
			//x += *(int *)((char *)p + a + 0x0 + 0x0 + x); // 111% vs 133% <- ops!
			// 117% vs 133%
			// ===============================================================
			x += *(int *)((char *)p + a + 0x0 + 0x8 + x);
			x += *(int *)((char *)p + a + 0x8 + 0x10 + x);
		}
	A_END(2)

	// ������ ����������� �� �������
	Ax_OUT("NORMAL vs FIRST-to-END", 0, 1);
	Ax_OUT("NORMAL vs OPTIMIZED   ", 0, 2);

	return x+y;
}
