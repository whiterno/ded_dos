/*----------------------------------------------------------------------------
 *
 *				������ ��������������� ������� ������������������
 *			��� ��������� �������������� ������ � ���� � �� �� ���-�������
 *			==============================================================
 *
 * Build 0x001 06.08.2002
----------------------------------------------------------------------------*/
// ������������
#define N_ITER			466			// ���-�� ��������
									// ������������ ���� ������������
									// LINE_SIZE*N_ITER ���� ���-������,
									// �.�. � ������ ������ 466*64 = ~30 Kb

#define CACHE_BANK_SIZE	(4*K)		// ������ ���-�����

#define LINE_SIZE		64			// ����������� ��������� ������ ���-�����

#define BLOCK_SIZE		((CACHE_BANK_SIZE+LINE_SIZE)*N_ITER) // ������ �����

#include <DoCPU.h>					// ���������� ���������


//----------------------------------------------------------------------------
//
//					�������, �������������� ��������� ���-�����
//
//----------------------------------------------------------------------------
int over_assoc(int *p)
{
	int a;
	volatile int x=0;

	// ��������: top-level ���� ��������, ��������� ���������
	// � ��� ���� ��������� ���� ���� 10 ���
	for(a=0; a < N_ITER; a++)
		// ������ ������ � ����� 4 Kb, � ����������
		// �  ��  P-II/P-III/P-4  �  ��  AMD Athlon
		// ������  ���������  ��������� ���� � ����
		// ��� ��������;
		// ���������, �������������� ����� 12 �����
		// �����������  ������ �� Athlon n���������
		// ��� �� �������
		//x+=*(int *)((int)p + a*CACHE_BANK_SIZE);
		*(int *)((char *)p + a*CACHE_BANK_SIZE)=x;

	return x;
}

//----------------------------------------------------------------------------
//
//					�������, ������������� ��� ����������
//
//----------------------------------------------------------------------------
int optimize(int  *p)
{
	int a=0;
	volatile int x=0;

	// ��������: top-level ���� ��������, ��������� ���������
	// � ��� ���� ��������� ���� ���� 10 ���
	for(a=0; a < N_ITER; a++)
	{
		// ������ �����  �  �����  CACHE_BANK_SIZE+LINE_SIZE
		// �.�. � ������ ������ 4096+64=4160 ����;
		// ��������� ������������ ������ ���� ����� ��������
		// ��  ��  �����  ����������  �  ����������  �������
		// ���-����� �� ��� 100%
		//x+=*(int *)((int)p + a*(CACHE_BANK_SIZE+LINE_SIZE));
		*(int *)((char *)p + a*(CACHE_BANK_SIZE+LINE_SIZE))=x;
	}
	return x;
}


main()
{
	int *p;
	int a = 0;

	PRINT("= = = ������������ ��������� ���-������ = = =\n");
	PRINT_TITLE;

	// �������� ������
	p = (int *)_malloc32(BLOCK_SIZE);

	// ����������������� �������
	A_BEGIN(0)
		a+=over_assoc(p);
	A_END(0)

	// ���������������� �������
	A_BEGIN(1)
		a+=optimize(p);
	A_END(1)

	// ����� ����������� �� �����
	Lx_OUT("",Ax_GET(1),Ax_GET(0));

}
