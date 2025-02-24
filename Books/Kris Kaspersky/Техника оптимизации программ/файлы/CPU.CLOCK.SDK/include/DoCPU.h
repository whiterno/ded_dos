/*----------------------------------------------------------------------------
 *
 *							D	0		C	P	U
 *						S				D				K
 *						=================================
 *
 *				� � � � � � � � � � � � � "DoCPU CLOCK" � � � � � � 1.1
 *				=======================================================
 *
 *										��� ����������� �������������
 *										� ����� "������� ����������� ��������"
 *										by ���� ���������	kpnc@programme.ru
															kk@sendmail.ru
-------------------------------------------------------------------------- */


/*============================================================================
 *
 *						���������� ����� ��������� ������
 *
 *	$Fcpu:xxx				-	�������������� ������� �������� CPU
 *	$NoSort					-	�� ��������� ����������
 *
 *	$DEBUG.print.Fcpu		-	���������� ������ �������� CPU
 *
 *
 *
============================================================================*/


/*============================================================================
 *
 *
 *	A_NITER				-	������� ��� ������ ������������� ��������

_NORDTSC
 * TITLE				-	���������, ��������� ������ PRINT_TITLE
============================================================================*/
 
// ������ ����������
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ���������� DoCPU
//#define __DOS__
#include "CPU.ini"
#include "PC.ini"


// ���������� ����� �����������
#undef _TEXT
#ifdef __DOS__
	#define TITLE "�⨫�� � ����� \"��孨��  ��⨬���樨  �ணࠬ\"  /* �������� ࠡ�祥 */"
#else
	#define TITLE "������� � ����� \"�������  �����������  �������\"  /* �������� ������� */"
#endif

#define ERR_OEM	"-ERR translates a string into the OEM-defined character set"
#define TEXT_PIPE	_TEXT("* ����� ��������� ������ ���� ������������� � ����\n")

#ifndef A_NITER					// ������� ��� ������ ������������� ��������
	#define A_NITER			8
#endif


#ifndef DoCPU_BUFF_N			// ������������ ���-�� ���������� �������
	#define DoCPU_BUFF_N	0x10
#endif

// ����������������� �����������
#define TIME_TO_WAIT		100
#define MAX_STR_LEN			256
#define DoCPU_BUFF_SIZE		A_NITER

#define	MAX_GRAPH_LEN	80
#define	GRAPH_CHAR		":"



#define A_BREAK __asm{int 0x3}

/*----------------------------------------------------------------------------
 *
 *						�������� ���������� �������
 *
----------------------------------------------------------------------------*/

// ���������� ������� ������� �������������� �� DoCPU.obj
// ------------------------------------------------------
#ifdef __cplusplus
	extern "C" int*	__cdecl	DoCPU(void* x);
	extern "C" void	__cdecl	A1(unsigned int *t);
	extern "C" int	__cdecl	A2(unsigned int *t);
#else
	extern int*		__cdecl	DoCPU(void* x);
	extern void		__cdecl	A1(unsigned int *t);
	extern int		__cdecl	A2(unsigned int *t);
#endif

// ����� �� ������� ������������ windows.h ���������� ��� ����������� ����
#ifndef _WINDOWS_
	extern int __stdcall Sleep(int a);
	extern int __stdcall GetTickCount(void);
	extern int __stdcall CharToOemA(char *src, char *dst);
#endif

// ERROR - ���� ����� ����.
#ifdef ERROR
	#undef ERROR
#endif

// ??? 
extern void _null();


/*----------------------------------------------------------------------------
 *
 *						����������� ����������
 *						======================
 *										��� ������������� ������ clock.h
 *
 * ��������:
 *			�� ����������� ������ ���������� ������ ����� ��������,�.�. ���
 *			����� ���������� � ��������� �������!
----------------------------------------------------------------------------*/
static unsigned int	DoCPU_AL_Rool;						// ���� �������� AL
static unsigned int	DoCPU_tmp, DoCPU_tmp0, DoCPU_tmp1;	// ��������� �����.

static unsigned int	DoCPU_t[DoCPU_BUFF_N];
static unsigned int	DoCPU_buff[DoCPU_BUFF_N][DoCPU_BUFF_SIZE];

static char			DoCPU_v[]="|/-\\";					// ������� "���������"
static char			DoCPU_s[MAX_STR_LEN];				// ��������� �����

float				DoCPU_float_tmp;
static int			CPU_CLOCK = 0;

static unsigned int			DoCPU_vcp=0;
static int*			p_cache=0;



/* ---------------------------------------------------------------------------
 *
 *							"����������" ���������
 *
----------------------------------------------------------------------------*/
#define	K			1024
#define	M			(1024*K)
#define G			(1024*M)
#define T			(1024*G)
#define	Mega		1000000
#define	Bit			8



/* ---------------------------------------------------------------------------
 *
 *							������� ������ ������
 *
----------------------------------------------------------------------------*/
#define _HUGE		(512*M)
#define _LARGE		(MAX_AVIAL_MEM)
#define	_NORMAL		(L2_CACHE_SIZE*10)
#define _MEDIUM		(L2_CACHE_SIZE/2)
#define _SMALL		(L1_CACHE_SIZE/2)

#define MAX_CACHE_LINE_SIZE												\
					MAX(L1_CACHE_LINE_SIZE, L2_CACHE_LINE_SIZE)

#define MAX(a,b)	(((a)>(b))?a:b)
#define MIN(a,b)	((a>b)?b:a)


/*  -----------------------------------------------------------------------

                        ����������� ������� �����

    ---------------------------------------------------------------------- */
//  ����������� ������� ��������������� �����
//  ������� ��������� ������������:
//  _���������_�����_  (SMALL)  -   �����, ���������� ����������� � L1-����
//  _���������_�����_  (MEDIUM) -   �����, ���������� ����������� � L2-����
//  _�������_�����_    (LARGE)  -   �����, ������� ������������� ������ L2 ����
//  _����������_�����_ (HUGE)   -   �����, ������� ������������ ���-�� RAM

// ������������ ������ ������� ����������� _BLOCK_SIZE ������������� ������
// �� ������������ ������ [__LARGE|__MEDIUM|__SMALL] � ������� ���� �����. CPU
// �� ��������� ������� ������� ����
// ����������� ������� ����� ����� ���������� ��� ����� #define �������������-
// �� � ������ ���������, ��� � ����� ���� /D ��������� ������ ����������� VC
// ��������: cl bla-bla-bla.c /D__MEDIUM


#ifdef __HUGE
	#define _BLOCK_SIZE _HUGE
#endif

#ifdef __LARGE
	#define _BLOCK_SIZE _LARGE
#endif

#ifdef __MEDIUM
	#define _BLOCK_SIZE _MEDIUM
#endif

#ifdef __SMALL
	#define _BLOCK_SIZE _SMALL
#endif

// ������� ����� �� ���������
#ifndef _BLOCK_SIZE
	#define _BLOCK_SIZE _LARGE
#endif



/*----------------------------------------------------------------------------
 *
 *							������ ������� ����������
 *
----------------------------------------------------------------------------*/

// ������� ����� ����� ������� ����������
// --------------------------------------
//
//		������� _[BEGIN | END]_[RDTSC|CLOCK]_  ��������  ����������  �������
// ������� ������ �������  ����������  ��������������  ���������  ���������.
// ��� ��������������� �������! � ������� �� ��������� �� �� ����� ���������
//
//		������ ������� �������������� � ����� DoCPU_buff � "�����", ��������
// ���������� t � �������, ������������� ��������� ���������� DoCPU_AL_Rool.
#define	_BEGIN_RDTSC(t)	A1(&DoCPU_t[t]);
#define	_END_RDTSC(t)	DoCPU_tmp = A2(&DoCPU_t[t]);\
						DoCPU_buff[t][DoCPU_AL_Rool] = DoCPU_tmp;

#define	_BEGIN_CLOCK(t)	DoCPU_t[t] = clock();
#define	_END_CLOCK(t)	DoCPU_tmp = clock() - DoCPU_t[t];\
						DoCPU_buff[t][DoCPU_AL_Rool] = DoCPU_tmp;


// ����������� ����� ������� ����������
// ------------------------------------


//		������   L_BEGIN   ������   �����������    �����    ������   ������,
// � L_END - ����������� ����� ����� ������ ������� ����������.
//
//		����   ������   �����������   __NORDTSC__  ��  ���   ������  �������
// ������������  ��������� ������, �   ���������  ��  ������  (�� ���������)
// ������� RDTSC
//
//		������� L_BEGIN/L_END ������������� ������������ ������ � ���� �����
// AL_BEGIN -- AL_END! � ������ �������� ����� �����  ��������� ����  �  ���
// �������, ����� ����� ������ �� A_NITER �������� ��������������  ���������
// ���������� ��������� ������������ ��������, ����� ����������  �������  ��
// ������ �����������. ���� �� ������������ ������� A_BEGIN/A_END
#ifdef _NORDTSC
	#define L_BEGIN(t)	_BEGIN_CLOCK(t)
	#define L_END(t)	_END_CLOCK(t)
#else
	#define L_BEGIN(t)	_BEGIN_RDTSC(t)
	#define L_END(t)	_END_RDTSC(t)
#endif

//		������� UL_BEGIN/UL_END ������������ ����� �� �� �����, ��� L_BEGIN/
// L_END, �� ����� ��������� ������ ����� ����� �������������� �� ������  ��
// ������ ����������, �� � � ���� ���������� ���������.
//
//	ARG:
//		t	-	������ ������ ��� ������ ����������� �������
//		tt	-	��������� ������. 
//				  0 : ������������ RDTSC
//				!=0 : ������������ ��������� ������
#define	UL_BEGIN(t,tt)	if (tt) {_BEGIN_CLOCK(t);}	else	{_BEGIN_RDTSC(t);}
#define	UL_END(t,tt)	if (tt) {_END_CLOCK(t);	}	else	{_END_RDTSC(t);  }



// ����������� ������������  ������ �������������� ��������� A_NITER ���
// ---------------------------------------------------------------------

//		�������  AL_BEGIN/A_END  ������������ ���������� ������� A_NITER ���
// � ��������: ���������� ����� ������� �������� � ���������� DoCPU_AL_Rool,
// ������������ ������� ��� ������ �����������  ������� �������� [U]L_BEGIN/
// [U]L_END
#define	AL_BEGIN		for(DoCPU_AL_Rool = 0;\
						DoCPU_AL_Rool < A_NITER; DoCPU_AL_Rool++ )\
						{
#define	AL_END			}


//		�������  A_BEGIN/A_END   ��������   �����������   ������������   ���
// ��������  �����������   �����   ������.  ���  ������������  �������������
// �������� A_NITER ���, ������� ����� ���������� ������ ��������
//
//		��������: ������ �� ����� ���� ����������!!!!
#define	A_BEGIN(t)		AL_BEGIN; L_BEGIN(t);
#define	A_END(t)		L_END(t); AL_END;


// ���������� ����������� ������� � �� ���������
// ---------------------------------------------

//		������ Ax_GET ����������  �������  �����  ����������  ��������������
// ���������, ����������� ���������� ������ t.
#define	Ax_GET(t)		cycle_mid(DoCPU_buff[t],0)

//		������  Lx_GET  ����������  _�������_  ��������  ������  �����������
// ����� t. ��������: �� ����������� ���� ������, ���� �� ��  ������� � ���,
// ��� ������ �� �������!
#define	Lx_GET(t)		DoCPU_buff[t][DoCPU_AL_Rool]

//		������   Ax_GET_MIN   ����������    �����������   �����   ����������
// �������������� ���������, ����������� ���������� ������ t.
#define	Ax_GET_MIN(t)	cycle_min(DoCPU_buff[t],0)

//		������ Ax_GET_MIN ����������  ���� �����   ���������� ��������������
// ���������, ����������� ���������� ������ t.
#define	Ax_GET_MOD(t)	cycle_mod(DoCPU_buff[t],0)


//		������ ALx_GET_MIN ���������� ����� ������ ��������������  ���������
// ��������� ����������� ������ t � tt �������� ����������
#define	ALx_GET(t,tt)	DoCPU_buff[t][DoCPU_AL_Rool*0 + tt]


/*----------------------------------------------------------------------------
 *
 *								����� �� �����
 *
----------------------------------------------------------------------------*/

// ������������� WIN --> MS-DOS
// ----------------------------
//
//	-	�� ������ 1.1 ������ ���� �������������� ��� ������ ��������,
//		���������� win-��������� �� �������
//
//	-	� ������ 1.1 �������������� ��������� ��� ������������
//		��������: ������� ������������ ������� ����!
#define _TEXT(a)		(CharToOemA(a,DoCPU_s)?&DoCPU_s[0]:ERR_OEM)


// ������� ������ ������ �� �����
// ------------------------------

//		������  PRINT  ������������ ������ s � DOS-��������� � ������� �� ��
// ��������. ����� ������� PRINT �� ���������������� � ����.
//		��� ����������������� ������ �������������� ������� �������� printf
#define	PRINT(s)		fputs(_TEXT(s),stderr);

//		������ ERROR ������� ������ s �� ��������, ������ � ��������� ������
#define	ERROR(s)		{ PRINT(s); PRINT("\x7"); return 0; }

//		������ PRINT_TITLE ������� ��������� ��������� � (c)
#define	PRINT_TITLE		PRINT("* "); PRINT(TITLE);PRINT("\n");

//		������  PRINT_PIPE  �������� �  ��������������  �������������  �����
// ���������� � ����. ��� ������� ��� ��� ����������, ������� ��������� ����
// ����� � ���� �������,  ������������ �� �����������  ��  ��������������  �
// MS Graph
#define	PRINT_PIPE		PRINT(TEXT_PIPE);



// ������� ������ ����������� ������� �� �����
// -------------------------------------------

//		������ L1_OUT ������� �� ����� ������ s � ������� 100%
// ������ �� ������������ � ���� � Lx_OUT ��� ������  ������������� ��������
// ���������� ��������� ������������� ���������� ���������
#define	L1_OUT(s)			printf("%s : 100%%\n",_TEXT(s));

//		������ Lx_OUT ������� �� ����� ������ s � ��������� base � val
#define	Lx_OUT(s,base,val)	printf("%s : %3.1f%%\n",\
								_TEXT(s),(float)val/(float)base*100);

//		������ A1_OUT �� �� �����, ��� � L1_OUT
#define	A1_OUT(s)			L1_OUT(s)

//		������ A1_OUT ������� �� ����� ������ s � ����� ���������� ���������
// ���������,   �����������  �����������  ������  val  ������������  �������
// ���������,   �����������  �����������  ������  base.
#define	Ax_OUT(s,base,val)	Lx_OUT(s, Ax_GET(base), Ax_GET(val))


//		������ A_LIST_ITER ������������� ��� ���������� �������  �����������
// ����� t
#define A_LIST_ITER(t)		printf("LIST_ITER {\n");						\
							for(DoCPU_tmp=0;DoCPU_tmp<A_NITER;DoCPU_tmp++)	\
								printf("\t%d\n",DoCPU_buff[t][DoCPU_tmp]);	\
							printf("} LIST_ITER\n");


//		������  A_LIST_ITER_AS_TABLE  �������������  ���  ����������  �������
// ��������� ���������, ����������� �����������  ������ t �  ���������  ��  �
// ���� �������, ��������� ��� �������������� � MS Graph
#define A_LIST_ITER_AS_TABLE(s,buf)											\
							printf(s);										\
							for(DoCPU_tmp=0;DoCPU_tmp<A_NITER;DoCPU_tmp++)	\
								printf("\t%d",buf[DoCPU_tmp]);				\
								printf("\n");
/*
// #define	Lx_OUTx(s,buf_base,buf_val,x_factor)\
// printf("%s : %d%%\n",s,100*(cycle_mid(DoCPU_buff[buf_val],0)/x_factor)/
// (cycle_mid(DoCPU_buff[buf_base],0)/x_factor));
// #define	Lx_OUT_DEBUG(s,buf_base,buf_val) printf("%s : %d%%, %d%%\n",s,cycle_mid(DoCPU_buff[buf_val],0),cycle_mid(DoCPU_buff[buf_base],0));
*/


// ������� ����������� �������� ���������
// ======================================

//		������, ��������� ���������� "���������"
#define VVV	if (DoCPU_vcp >= strlen(DoCPU_v))	DoCPU_vcp = 0;				\
			sprintf(DoCPU_s,"\rDoCPU:%c\r",DoCPU_v[DoCPU_vcp]);				\
			PRINT(DoCPU_s); DoCPU_vcp++;

//		������, ��������� ������� ������� � �������� � ���������
#define PRINT_PROGRESS(x)	sprintf(DoCPU_s,"\rProgress : %3d%%\r",(x));	\
							PRINT(DoCPU_s);


// ������� ������ �������� "������"
// --------------------------------
// ARG:
//		size	-	������ ������� ��������� ��� ������
//
//		n_sym	-	���������� �������� � ������ (NULL == 80)
//
//		max		-	������������ �������� size.
//					==  0 : max ����� ���������� �������� ������� size
//					== -1 : ����� ����������� form_factor � �������
//
//		nl		-	==  0 : ���������������� �����,
//					==  1 : ���������������
void online_graph(int size, int n_sym, int max, int nl)
{
	int		a;
	int		form_factor;
	static	graph_size = 0;

	// ����� graph_size � ����
	if (max == -1) { graph_size = 0; return; }

	// ���������� graph_size �� ������� size
	if ((!max) && (!graph_size)) graph_size = size * 2;

	// ���������� ����� ���������
	if (!n_sym) n_sym = MAX_GRAPH_LEN;

	// ���������� ����-�������
	if (!max) form_factor = graph_size / n_sym;
		else
	form_factor = max / n_sym;

	// ������ ��������� ��������������� ������
	if (nl)
	{
		size = 10*log(size);
		form_factor=1;
	}

	if (!form_factor) form_factor = 1;

	size /= form_factor;
	// ����� ���������
	for (a = 0; a < size; a++)
	{
		if (a >= (n_sym-3))
		{									// ����� �� ������� ���������
			if (size > (n_sym-3)*3) printf(">");
			if (size > (n_sym-3)*2)	printf(">");
			if (size > (n_sym-3)*1)	printf(">");
			
			
			break;
		}
		printf(GRAPH_CHAR);
	}
	printf("\n");
}

//		������ ��� �������� ������ online_graph c ����������� �� ���������
#define GRAPH(a) online_graph(a,0,0,0);

//		������ ��� ������������ online_graph
#define RE_GRAPH online_graph(0,0,-1,0);


/*----------------------------------------------------------------------------
 *
 *										����
 *
----------------------------------------------------------------------------*/

// ������ ������� ������. ����� ��?
#define A_READ(p, size_p)	for(DoCPU_tmp=0; DoCPU_tmp < size_p; DoCPU_tmp += sizeof(int))\
													DoCPU_tmp1+=((int *)(p+DoCPU_tmp))[0];




/*----------------------------------------------------------------------------

						������� ������ � ����� � �������

----------------------------------------------------------------------------*/

//		������ _malloc32 �������� x ���� ������, ����������, ���  ����������
// ���� ����� ���������� � ������, �������� 32
#define	_malloc32(x)	(( (((int) malloc((x)+0x20)) + 0x20) & 0xFFFFFFE0  ));


//		������ _align32 ����������� ��������� p �� ������, �������� 32 ����
#define	_align32(p)		(( (((int) (p)) + 0x20) & 0xFFFFFFE0  ));


//		������ CLEAR_L2_CACHE  �������� ��� ������� ������ �������, ��������
// ����� ���������� � ���� ������.
int CLEAR_L2_CACHE()
{
	int a;
	int x = 0;
	
	if (!p_cache) p_cache = (int *) malloc(_NORMAL);

	for (a = 0; a<_NORMAL; a++)
		x += *(int *)((int)p_cache+a);
	
	return x;
}



int CLEAR_ALL()
{
		// !!!������ �� ����������!!!
		return 0;
}


//		������ ���� ���������� ������ ������ (store buffers) �� P-III 
void _DoCPU_a_fflush()
{
#ifdef __FLUSH__
	_DoCPU_a_flush();
#endif
}



#define RM 	for(DoCPU_tmp = 0; DoCPU_tmp < 10; DoCPU_tmp++) DoCPU_float_tmp+=0.99;



/*----------------------------------------------------------------------------

								��������� �������

----------------------------------------------------------------------------*/

// * FUNC: int getargv(char *arg_name, char *arg_val)
// * ������� �������� ������� � ��������� ������ ���������
//
//		���� ��������  arg_name ������  �  ���������  ������,   ��   �������
// ���������� ��������������� ��������;
//
//		���� �������� arg_name ����������� � ��������� ������ (��� ��� �����
// ��������� MAX_STR_LEN), ������� ���������� -1;
//
//		���� �������� ����� ����,   ����������   ��   ���������   ����������
// (���-�� ���� "/XXX:666"), ���  ���������  �������� ������������ � arg_val
// ��� �������, ��� arg_val �� ����� ����;
//
//		����  arg_val==NULL �� ������� �������: ��� ���� ��������� �� �����;
//
//		��� ������� ����� �������  ��� ��  ��������   �������  ���  ��������
// �������� (���������� �� atol);
//
//		���������: ���� ������� ���������� 0, �� �������� ������������: ����
// �������� ������ �� ����� �����, ���� ���� ���������� ��������, ����  ����
// ������������� ����� ����. � ����� ������  �� ������ ���������  ����������
// arg_val ��� ��������� ��������
int getargv(char *arg_name, char *arg_val)
{
	int				a;
	int*			p;
	unsigned int	c;
	
	char			buf_arg[MAX_STR_LEN];
	char			buf_val[MAX_STR_LEN];

	#ifndef __argv		
		char** __argv;
		int __argc=0;
	#endif

	if (!arg_name) return -1;

	if (arg_val) arg_val[0] = 0;	// ������������

	// ������� ���� ����������
	for (a = 1; a < __argc; a++)
	{
		if (strlen(__argv[a]) >= MAX_STR_LEN) return -1;

		// �������������� ������ ��������� � ����������� ��� ����� � buf_arg
		for (c=0;c!=(1+strlen(__argv[a]));c++)
		{
			if (__argv[a][c] == ':') { buf_arg[c] = 0; break; }
				buf_arg[c] = __argv[a][c];
		}
		
		// ����������� ����� (���� �� ����) � buf_val
		if (__argv[a][c] == ':') strcpy(buf_val, &__argv[a][c +1]); else buf_val[0] = 0;

		// ���� ��� ��� ��������, ������� ��� ����� - ����������� ��� ��������� � arg_val
		if (!strcmp(arg_name,buf_arg))
		{
			if (arg_val) strcpy(arg_val,buf_val);
			return atol(buf_val);
		}
	}
	return -1;
}

//		������ IFHELP ���  ������� � ��������� ������  ����� "/?" ������� ��
// �������� ������ s � ��������� ������ ���������
#define	IFHELP(s)	if (getargv("/?",0)!=-1){ PRINT(_TEXT(s)); return 0; }

//		������  GETARGV   ������������   �����   "�������"  ������� getargv.
// ��� �������  ����� s  �  ���������  ������  ��  �������  ���  ��������  �
// ���������� val. ���� ����� s � ��������� ������ ���, �� ���������� val ��
// ����������.
#define	GETARGV(s,val)	DoCPU_tmp=getargv(s,NULL);							\
						if (DoCPU_tmp!=-1) val=DoCPU_tmp;

//#define A_FLUSH _DoCPU_a_flush();
//#define A_FFLUSH _DoCPU_a_fflush();
//#define A_WAIT  Sleep(TIME_TO_WAIT);


/*----------------------------------------------------------------------------
 *
 *						������� ���� ��������������
 *
----------------------------------------------------------------------------*/
int*	A0(void* x)
{
		Sleep(100);
		return DoCPU(x);
}


/*----------------------------------------------------------------------------
 *
 *						��������� ������� ���� ��������������
 *
----------------------------------------------------------------------------*/
// ������� ���������� �������� ������� ���������� � ����������
int GetCPUclock(void)
{
	#define to_time 1

	int				tmp;
	unsigned int	t;
	volatile		time_t timer;

	tmp = getargv("$Fcpu",0);

	if (tmp <= 0)
	{
		if (!(tmp=CPU_CLOCK))
		{
			timer = time(NULL);
			while(timer == time(NULL));
			timer = time(NULL);

			A1(&t);
				while( timer+to_time > time(NULL));
			t=A2(&t);

			tmp=((double)t) / ((double)1000000) / ((double)to_time);

			CPU_CLOCK=tmp;
		}
	}
	
	if (getargv("$DEBUG.print.Fcpu",0)!=-1) printf(">DEBUG:Fcpu=%d\n",tmp);
	return tmp;
}

// ������� ����������� ����� � �������
float cpu2timeu(unsigned int ticks, int tt)
{
	if (tt)
		return ((float )ticks)/CLOCKS_PER_SEC;

	if (!CPU_CLOCK) CPU_CLOCK=GetCPUclock();
		return ((float )ticks)/CPU_CLOCK/1000000;
}


#ifdef _NORDTSC
	float cpu2time(int ticks)
	{
		return cpu2timeu(ticks, 0);
		
	}
#else
	float cpu2time(int ticks)
	{
		return cpu2timeu(ticks, 1);
	}
#endif


/*----------------------------------------------------------------------------
 *
 *						������� ������� ��������� �����������
 *
----------------------------------------------------------------------------*/
int cycle_min(int *buff, int n_buff)
{
	int a, tmp;
	if (!n_buff)	n_buff = DoCPU_BUFF_SIZE;

	tmp = buff[0];
	for(a = 0; a < n_buff; a++)
		if (buff[a] < tmp) tmp = buff[a];
	return tmp;
}

int cycle_max(int *buff, int n_buff)
{
	int a, tmp;
	if (!n_buff)	n_buff = DoCPU_BUFF_SIZE;
	
	tmp = buff[0];
	for(a = 0; a < n_buff; a++)
		if (buff[a] > tmp) tmp = buff[a];

	return tmp;
}


	int _compare(unsigned int *arg1, unsigned int *arg2 )
	{
		if (*arg2 >= *arg1) return -1;
		// else 
		return +1;
	}

unsigned int cycle_mid(unsigned int *buff, int nbuff)
{
	int a, xa = 0;
	if (!nbuff) nbuff = A_NITER;

	// ��������� ������ �������
	buff = buff + 1; nbuff--;

	if (getargv("$NoSort",0) == -1)
		qsort(buff, nbuff, sizeof(int),\
				(int (*)(const void *,const void*))(_compare));

	for (a = nbuff/3; a < (2*nbuff/3); a++)
		xa += buff[a];

	xa /= (nbuff/3);

	return xa;
}


	int _Search(int *array, int array_size, int val)
	{
		int a;

		for (a = 0; a < array_size; a++)
		{
			if (array[a] == val) return a;
		}
	return -1;
	}

int cycle_mod(int *buff, int n_buff)
{
	int a,b,tmp;
	int *val_array;
	int *cnt_array;
	int array_size = 0;

	if (!n_buff) n_buff=DoCPU_BUFF_SIZE;

	val_array = (int*) malloc(n_buff*sizeof(int));
	cnt_array = (int*) malloc(n_buff*sizeof(int));

	// �������� ���������� 
	for (a=0;a<n_buff;a++)
	{
		tmp=_Search(val_array, array_size, buff[a]);
		if (tmp==-1) {array_size++;val_array[array_size-1]=buff[a];cnt_array[array_size-1]=1;}
			else
		{ cnt_array[tmp]+=1; }
	}

	// ���� �������� ���������� �������
	tmp=cnt_array[0];b=0;
	for (a=1;a<array_size;a++)
	{
		if (cnt_array[a]>tmp) {tmp=cnt_array[a]; b=a;}
	}

	tmp = val_array[b];
	free(val_array);
	free(cnt_array);
	return tmp;
}


float cycle_average(int* buff, int n_buff)
{
	int a;
	float avrg = 0;

	if (!n_buff) n_buff=DoCPU_BUFF_SIZE;

	for (a = 0; a < n_buff; a++)
		avrg += buff[a];

	return avrg/n_buff;
}


