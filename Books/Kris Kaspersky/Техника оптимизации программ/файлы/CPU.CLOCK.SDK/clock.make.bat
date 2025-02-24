@ECHO OFF
REM ================================ �⨫�� �������樨 =======================
REM 1) ��������� �� �����������:
REM  - ��६����� ���㦥��� CLOCKSDK ������ 㪠�뢠�� �� ���� � ������ CLOCKSDK
REM    _���_ ��������� ���� � �����
REM ===========================================================================
SETLOCAL

REM /* -----------------------------------------------------------------------
REM
REM                          � � � � � � � � � � � � 
REM
REM -------------------------------------------------------------------------*/
REM
IF NOT #%CLOCKSDK%#==## GOTO INSTALL_OK
	SET CLOCKSDK=F:\.optimize\bin\clock
	REM 		 ^^^^^^^^^^^^^^^^^^^^^^
	REM ���� � CLOCK SDK �� 㬮�砭��
	REM ECHO ��������! �����४⭠� ���⠫����!
	REM ECHO ��६����� �।� CLOCKSDK �� ��।�����!
:INSTALL_OK

SET COMPIL=MSVC
REM ^^^^^^^^^^^ ��� ��������� �� 㬮�砭��
REM             �����ন������ ᫥���騥 ����������:
REM             MSVC : Microsoft Visual C++
REM             BC   : Borland C++
REM             WPP  : Watcom  C++


SET MSVC_NAME=Microsoft Visual C++
SET BC_NAME=Borland C++
SET WPP_NAME=WATCOM

SET COMPIL_NAME=MSVC_NAME
REM ^^^^^^^^^^^ ��� ��������� �� 㬮�砭��

SET OPTIONS=
REM ^^^^^^^^^^^^^^^
REM 			����� �������樨 �� 㬮�砭��
REM 			�����ন������ ᫥���騥 ०���:
REM				-release : ��������� � ��⨬���樥�
REM				-debug   : ��������� � �⫠��筮� ���ଠ樥�
REM				##		 : ��������� �� ��䫮��

REM call-back for KPNC.SHELL
:KPNC_SHELL
IF #%1#==#__class#    GOTO END
IF #%1#==#__typeinfo# GOTO USAGE
IF #%1#==#/?#         GOTO HELP

REM // TITLE
ECHO * CLOCK PROJECT MAKER * by Kris Kaspersky Version 1.0 beta AT 23-07-2001
ECHO (�⨫�� � ����� "��孨��  ��⨬���樨  �ணࠬ" /* �������� ࠡ�祥 */)

IF #%CLOCKSDK%#==## GOTO ERR_NO_CLOCKSDK

REM /* -----------------------------------------------------------------------
REM
REM    � � � � � �   � � � � � � � � � �   � � � � � � � � �  � � � � � � 
REM
REM -------------------------------------------------------------------------*/
REM

GOTO P1

:PARSE 
	REM /*             ������ ��㬥�⮢ ��������� ��ப�                  */
	REM =====================================================================
	SHIFT

	:P1
	IF #%1#==## GOTO HELP

	:P11
	IF NOT #%1#==#-MSVC# GOTO P2
		SET COMPIL=MSVC
	SET COMPIL_NAME==MSVC_NAME
GOTO PARSE

	:P2
	IF NOT #%1#==#-BC# GOTO P3
		SET COMPIL=BC
		SET COMPIL_NAME=BC_NAME
GOTO PARSE

	:P3
	IF NOT #%1#==#-WPP# GOTO P4
		SET COMPIL=WPP
		SET COMPIL_NAME=WPP_NAME
GOTO PARSE

	:P4
	IF NOT #%1#==#-debug# GOTO P5
		SET OPTIONS=DEBUG
GOTO PARSE

	:P5
	IF NOT #%1#==#-release# GOTO P6
		SET OPTIONS=RELEASE
GOTO PARSE

	:P6
	IF NOT #%1#==#-nop# GOTO P7
GOTO PARSE

	:P7
	IF NOT #%1#==#-nocleartmp# GOTO P8
		SET nocleartmp=YES
GOTO PARSE

	:P8
	IF NOT #%1#==#-LIB# GOTO P9
		SHIFT
		SET MYLIB=%1
GOTO PARSE

	:P9

:DO_IY
REM /* -----------------------------------------------------------------------
REM
REM 				�஢�ઠ 䠩��� SDK �� ����⢮�����                 
REM
REM -------------------------------------------------------------------------*/
REM
	IF NOT EXIST %CLOCKSDK%\SOURCE\DoCPU.asm GOTO ERR_NO_DoCPU_ASM
	IF NOT EXIST %CLOCKSDK%\INCLUDE\DoCPU.h  GOTO ERR_NO_DoCPU_H

	REM /*                   ������ ��������� �������                       */
	REM ======================================================================
	ECHO ===== %~n1 �஥�� =====
	IF NOT EXIST %~n1.c   	GOTO ERR_C_NOT_EXIST
	IF #%~n1#==#code#		GOTO ERR_PRJ_NAME

	REM �뢮� ����� 䠩��� �஥��
	SET FILES_PRG=%~n1.c
	IF EXIST %~n1.mod  SET FILES_PRG=%FILES_PRG%;%~n1.mod
	IF EXIST %~n1.ini  SET FILES_PRG=%FILES_PRG%;%~n1.ini
	SET FILES_PRG=%FILES_PRG%;DoCPU.asm;DoCPU.h.
	ECHO [0/3] ����� �஥�� %FILES_PRG%

	REM  /*               �����⮢�塞 䠩�� � ��ᥬ���஢����               */
	REM =======================================================================
	COPY %CLOCKSDK%\SOURCE\DoCPU.asm %CLOCKSDK%\TEMP\ >NUL
	IF EXIST %~n1.mod COPY %~n1.mod  %CLOCKSDK%\TEMP\ >NUL
	IF EXIST %~n1.ini COPY %~n1.ini  %CLOCKSDK%\TEMP\ >NUL

	IF EXIST 		%~n1.mod ECHO INCLUDE %~n1.mod > %CLOCKSDK%\TEMP\code.inc
	IF NOT EXIST  	%~n1.mod ECHO ;                > %CLOCKSDK%\TEMP\code.inc

	IF EXIST 		%~n1.ini ECHO INCLUDE %~n1.ini > %CLOCKSDK%\TEMP\code.ini
	IF NOT EXIST  	%~n1.ini ECHO ; 	 		   > %CLOCKSDK%\TEMP\code.ini


	REM /*                     ��⮢�� 䠩� clock.clear.bat                     */
	REM ===========================================================================
	ECHO ECHO ��� 䠩� ᣥ���஢�� by clock.make.bat ��� ���⪨ �६����� 䠩��� �஥�� %~n1 > %CLOCKSDK%\TEMP\clock.clear.bat

	ECHO DEL %CLOCKSDK%\TEMP\code.inc   > %CLOCKSDK%\TEMP\clock.clear.bat
	ECHO DEL %CLOCKSDK%\TEMP\code.ini  >> %CLOCKSDK%\TEMP\clock.clear.bat
	ECHO DEL %CLOCKSDK%\TEMP\DoCPU.asm >> %CLOCKSDK%\TEMP\clock.clear.bat

	IF EXIST %CLOCKSDK%\TEMP\%~n1.mod ECHO DEL %CLOCKSDK%\TEMP\%~n1.mod >> %CLOCKSDK%\TEMP\clock.clear.bat
	IF EXIST %CLOCKSDK%\TEMP\%~n1.ini ECHO DEL %CLOCKSDK%\TEMP\%~n1.ini >> %CLOCKSDK%\TEMP\clock.clear.bat

	ECHO IF EXIST DoCPU.obj DEL DoCPU.obj >> %CLOCKSDK%\TEMP\clock.clear.bat
	ECHO IF EXIST %~n1.obj  DEL %~n1.obj  >> %CLOCKSDK%\TEMP\clock.clear.bat

	IF OPTIONS==RELEASE ECHO IF EXIST %~n1.pdb DEL %~n1.pdb >> %CLOCKSDK%\TEMP\clock.clear.bat
	IF OPTIONS==RELEASE ECHO IF EXIST vc??.pdb DEL vc??.pdb >> %CLOCKSDK%\TEMP\clock.clear.bat


	REM  **********************   ��ᥬ����㥬 DoCPU ******************************
	REM  ==========================================================================
	ECHO [1/3] ��ᥬ����㥬 DoCPU.asm
	SET OOPS1=
	SET OOPS2=
	IF #%COMPIL%#==#MSVC#   SET OOPS1=/coff
	IF #%OPTIONS%#==#DEBUG# SET OOPS2=/Zi
	ml.exe /WX %OOPS1% %OOPS2% /X /c /nologo %CLOCKSDK%\TEMP\DoCPU.asm > %~n1.err

	IF NOT EXIST DoCPU.obj 	GOTO ERR_LINK
	DEL %~n1.err


	REM *************************   ��������㥬  *******************************
	REM =========================================================================
	ECHO [2/3] ��������㥬 �஥�� "%~n1" by %compil_name% %2 %3 %4 %5 %6 %7 %8 %9
	SET ERR_COMPIL=
	IF EXIST %~n1.exe 		DEL %~n1.exe

	REM ��।��塞: 祬 �㤥� �������஢���
	IF %COMPIL%==MSVC 	GOTO msvc
	IF %COMPIL%==BC 	GOTO bc
	IF %COMPIL%==WPP 	GOTO wpp
	GOTO ERR_UNK_COMPIL

	:MSVC
		SET OOPS=
		IF #%OPTIONS%#==#DEBUG# 	SET OOPS=/Zi
		IF #%OPTIONS%#==#RELEASE# 	SET OOPS=/Ox
		cl.exe %~n1.c %2 %3 %4 %5 %oops% /WX /nologo /I%CLOCKSDK%\INCLUDE\ /link DoCPU.obj USER32.LIB %MYLIB% >%~n1.err
	GOTO after_compil

	:BC
		SET OOPS=
		IF #%OPTIONS%#==#DEBUG# 	SET OOPS= REM !!! ������� �������������� !!!
		IF #%OPTIONS%#==#RELEASE# 	SET OOPS=-6 -O3

		call bcc32.bat %2 %3 %4 %5 -w! -I%CLOCKSDK%\INCLUDE\ %~n1.c DoCPU.obj USER32.LIB >%~n1.err
		GOTO after_compil

	:WPP
		SET OOPS=
		IF #%OPTIONS%#==#DEBUG# 	SET OOPS= REM !!! ������� �������������� !!!
		IF #%OPTIONS%#==#RELEASE# 	SET OOPS=/ox
		call wpp.bat %~n1.c %2 %3 %4 %5 -I%CLOCKSDK%\INCLUDE; DoCPU.obj USER32.LIB %MYLIB% >%~n1.err
	GOTO after_compil


:AFTER_COMPIL
	rem IF %errorlevel% GEQ 1 		SET ERR_COMPIL=TRUE

	REM /*                     �������� �६����� 䠩���                   */
	REM =====================================================================
	IF #%OPTIONS%#==#RELEASE# IF EXIST %~n1.pdb DEL %~n1.pdb
	IF #%OPTIONS%#==#RELEASE# IF EXIST %~n1.ilk DEL %~n1.ilk
	IF NOT "%NOCLEARTMP%"=="YES" GOTO DEL_TEMP
	ECHO [3/3] �६���� 䠩�� �� 㤠����, �.�. �ᯮ�짮��� ���� -nocleartmp

	GOTO CHK_ERR

	:DEL_TEMP
	ECHO [3/3] ����塞 �६���� 䠩��
	CALL %CLOCKSDK%\TEMP\clock.clear.bat

	:CHK_ERR
	IF #%ERR_COMPIL%#==#TRUE# 	GOTO err_compil
	IF NOT EXIST %~n1.exe 		GOTO ERR_COMPIL
	DEL %~n1.err

	ECHO [+OK] ��������� �ᯥ譮 �����襭�
	ECHO ::��ନ஢�� 䠩� %~n1.exe
	IF #%OPTIONS%#==#DEBUG#   ECHO ::� �⫠��筮� ���ଠ樥�
	IF #%OPTIONS%#==#RELEASE# ECHO ::�⪮����஢���� � ���ᨬ��쭮� ��⨬���樥�
	GOTO END

:HELP
	ECHO USAGE:
	ECHO clock.make.bat [-debug\-release\-nop\-BC\-WPP\-LIB] modname [arg1,arg2,..]
	ECHO       -debug      : generate debug info
	ECHO       -release    : maximum optimize   
	ECHO       -nocleartmp : �� 㤠���� �६���� 䠩��        
GOTO END

:USAGE
	ECHO CLOCK PROJECT MAKER     [clock.make.bat $opt $filename $user_opt] 
GOTO END

:ERR_LINK
	ECHO -ERR: �訡�� ��ᥬ���஢����. ��. %~n1.err
GOTO END

:ERR_COMPIL
	ECHO -ERR: �訡�� �������樨. ��. %~n1.err
GOTO END


:ERR_C_NOT_EXIST
ECHO -ERR: �᭮���� ����� %1 �� �������
GOTO END

:ERR_PRJ_NAME
ECHO -ERR: ����� �ᯮ�짮���� �㦥���� ᫮�� CODE ��� ������������ �஥�� 
GOTO END


:ERR_NO_DoCPU_ASM
ECHO.
ECHO -ERR: ��������� 䠩� %CLOCKSDK%\SOURCE\DoCPU.ASM
ECHo ��୨� 䠩� �� ���� � ������ ������!
GOTO END

:ERR_NO_DoCPU_H
ECHO.
ECHO -ERR: ��������� 䠩� %CLOCKSDK%\INCLUDE\DoCPU.h
ECHo ��୨� 䠩� �� ���� � ������ ������!
GOTO END


:ERR_NO_CLOCKSDK
ECHO.
ECHO -ERR: �� ������  ��६�����  ���㦥���  CLOCKSDK,
ECHO 㪠�뢠��� ���� � ������ CLOCKSDK ���  ���������
ECHO ���� � ����. ������� �� � ������ ������...
GOTO END

:ERR_UNK_COMPIL
ECHO -ERR: ��������� ��������� %compil%
GOTO END

ENDLOCAL
:END
set errorlevel=1
