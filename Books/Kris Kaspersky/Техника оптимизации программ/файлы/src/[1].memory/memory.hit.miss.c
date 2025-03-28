/* ---------------------------------------------------------------------

					������� ��� ��������� �����������
		������� ������� � ������ � ����������� �� ���� ������
		=====================================================

Build 0x001	26.04.2002 turkey
Build 0x002	24.05.2002 ��. standing.wave	
Build 0x003	24.05.2002 
------------------------------------------------------------------------ */

// ������������
#define BLOCK_SIZE		(128*K)		// ����� ��������������� �����
#define STEP_FACTOR		(32*1)		// �������� ���������� ����
#define MAX_STEP_SIZE	(6*K)		// ����������� ���������� ���
#define MAX_ITER		(BLOCK_SIZE/MAX_STEP_SIZE/4)

#include <DoCPU.h>

main()
{
	int a,b,i,x=0;

	int *p;

	// TITLE
	PRINT("=== ������������ memory hit, memory miss & memory organization ===\n");
	PRINT_TITLE;

	// �������� ������
	p=(int*) _malloc32(BLOCK_SIZE);

	// �����
	printf("STEP"); for(a=STEP_FACTOR;a<MAX_STEP_SIZE;a+=STEP_FACTOR)
		printf("\t%d",a); printf("\nTime:");

	for(a = STEP_FACTOR; a <= MAX_STEP_SIZE; a += STEP_FACTOR)
	{
		PRINT_PROGRESS(100*a/MAX_STEP_SIZE);
		AL_BEGIN;
		CLEAR_L2_CACHE();
		//for (i=0;i<=BLOCK_SIZE;i+=4*K) x += *(int *)((int)p + i);
			L_BEGIN(0);
				b=0;
					for(i=0;i<MAX_ITER;i++)
					{
						x += *(int *)((char*)p + b);
						x += *(int *)((char*)p + b + a);
						x += *(int *)((char*)p + b + a*2);
						x += *(int *)((char*)p + b + a*3);
						b+=a*4;
			}
			L_END(0)
		AL_END;
		printf("\t%d",Ax_GET(0));
	}
	printf("\n");
	return 0;
}