/*
	������, ��������������� ������� ����������� ���������� ������
	����� �������� ������ � ��������� ������, ����� ���
*/


#include <DoCPU.h>
#include <DoCPU.cmd.h>



#define _MEM_SET	0
#define _PAGE_SIZE 4*K

#define BLOCK_SIZE (8*M)

/* -------------------------------------------------------------------

						������������������ ������� 

--------------------------------------------------------------------- */
normal(char *p)
{
 	memset(p,0x666,BLOCK_SIZE);
	A_READ(p,BLOCK_SIZE);
}



	/* ----------------------------------------------------------------------

				���������������� �������
							(� �������������� ������������ ���������� ������)

	----------------------------------------------------------------------- */
nonstore(char *p, char *val)
{
	int a;
	for (a = 0; a < BLOCK_SIZE; a += 16)
	{
		__stream_cpy(p, val);
		p += 16;
	}
	A_READ(p-BLOCK_SIZE,BLOCK_SIZE);
}


	/* ----------------------------------------------------------------------

				����������������� �������
							(� �������������� MMX'���� ���������� ������)

	----------------------------------------------------------------------- */
mmx_store(char *p, char *val)
{
	int a;
	for (a = 0; a < BLOCK_SIZE; a += 8)
	{
		__mmx_cpy(p, val);
		p += 8;
	}
	A_READ(p-BLOCK_SIZE,BLOCK_SIZE);
}


main()
{
	int a,b,d;		
	char *z;
	char *p;
						
	// �������� ������
	p=(char *)_malloc32(BLOCK_SIZE*A_NITER);

  	z = (char *) _malloc32(sizeof(int));


	// ������������� ���������� ����������

	printf("\n_____Memset Optimization Using Movaps_____\n");
	PRINT_TITLE;

	A_BEGIN(0)
		normal(p);
	A_END(0)

	A_BEGIN(1)
		nonstore(p, z);
	A_END(1)


   	A_BEGIN(2)
   		mmx_store(p, z);
	A_END(2)

	RM;
	Ax_OUT("OPTIMIZED ", 0, 1);	
	Ax_OUT("PESSIZIZED", 0, 2);	
}