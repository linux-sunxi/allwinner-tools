@echo off
set pwd=%CD% 
:select_chips
echo chips: 0.sun4i 1.sun5i:
set /p chips=select   
if  %chips%==0     ( set chips=sun4i
					goto  select_platform
					)
if %chips%==sun4i  ( set chips=sun4i
					goto  select_platform
					)
if  %chips%==1     ( set chips=sun5i
					goto  select_platform
					)
if %chips%==sun5i  ( set chips=sun5i
					goto  select_platform
					)
echo your select is not exist
goto select_chips									
					
:select_platform

echo platform: 0.crane 1.linux:
set /p platform=select     
if  %platform%==0     ( set platform=crane
					   goto  select_board
					  )
if %platform%==crane  ( set platform=crane
					   goto  select_board
					  )
if  %platform%==1     ( set platform=linux
					   goto  select_board
					  )
if %platform%==linux  ( set platform=linux
					   goto  select_board
					  )
echo your select is not exist
goto select_platform
	
:select_board
if platform==crane goto board_for_crane
echo board: 0.aino 1.evb 2.evb-v12r 3.evb_v13 4.tvdevb:
set /p board=select     
if  %board%==0        ( set board=aino
					   goto  set_board_ok
					  )
if %board%==aino      ( set board=aino
					   goto  set_board_ok
					  )
if  %board%==1        ( set board=evb
					   goto  set_board_ok
					  )
if %board%==evb       ( set board=evb
					   goto  set_board_ok
					  )
if  %board%==2        ( set board=evb-v12r
					   goto  set_board_ok
					  )
if %board%==evb-v12r  ( set board=evb-v12r
					   goto  set_board_ok
					  )
if  %board%==3        ( set board=evb_v13
					   goto  set_board_ok
					  )
if %board%==evb_v13   ( set board=evb_v13
					   goto  set_board_ok
					  )
if  %board%==4        ( set board=tvdevb
					   goto  set_board_ok
					  )
if %board%==tvdevb    ( set board=tvdevb
					   goto  set_board_ok
					  )
echo your select is not exist
goto select_board

:board_for_crane
echo board: 0.aino 1.bk7011 2.evb 3.evb-v12r 4.evb_v13 5.onda_m702h6 6.onda_n507h5 7.t780 8.tvdevb
set /p board=select   
if  %board%==0               ( set board=aino
					           goto  set_board_ok
					         )
if %board%==aino             ( set board=aino
					           goto  set_board_ok
					         )
if  %board%==1               ( set board=bk7011
					           goto  set_board_ok
					         )
if %board%==bk7011           ( set board=bk7011
					           goto  set_board_ok
					         )
if  %board%==2               ( set board=evb
					           goto  set_board_ok
					         )
if %board%==evb              ( set board=evb
					           goto  set_board_ok
					         )
if  %board%==3               ( set board=evb-v12r
					           goto  set_board_ok
					         )
if %board%==evb-v12r         ( set board=evb-v12r
					           goto  set_board_ok
					         )
if  %board%==4               ( set board=evb_v13
					           goto  set_board_ok
					         )
if %board%==evb_v13          ( set board=evb_v13
					           goto  set_board_ok
					         )
if  %board%==5               ( set board=onda_m702h6
					           goto  set_board_ok
					         )
if %board%==onda_m702h6      ( set board=onda_m702h6
					           goto  set_board_ok
					         )
if  %board%==6               ( set board=onda_n507h5
					           goto  set_board_ok
					         )
if %board%==onda_n507h5      ( set board=onda_n507h5
					           goto  set_board_ok
					         )
if  %board%==7               ( set board=t780
					           goto  set_board_ok
					         )
if %board%==t780             ( set board=t780
					           goto  set_board_ok
					         )
if  %board%==8               ( set board=tvdevb
					           goto  set_board_ok
					         )
if %board%==tvdevb           ( set board=tvdevb
					           goto  set_board_ok
					         )
							 
echo your select is not exist
goto select_board							 
							 
:set_board_ok



echo chips    :%chips%
echo platform : %platform%
echo board    :%board%

pause


set crane_out=W:\work\android2.3.4\out\target\product\crane-evb\images
set linux_out=..\..\out
set TOOLS_DIR=%CD%\pctools\windows
set PATH=%TOOLS_DIR%\mod_update;%TOOLS_DIR%\fsbuild200;%TOOLS_DIR%\edragonex200;%PATH%
echo %linux_out%
echo %TOOLS_DIR%
echo %PATH%

if exist out (
    echo Delete old out dir , and create it again
    rmdir /s /q out
    mkdir out out\bootfs
) else (
    echo Create out dir
    mkdir out out\bootfs
)

if exist eFex (

    rmdir /s /q eFex
    xcopy /q /e chips\sun4i\eFex\* eFex\
) else (

   xcopy /q /e chips\sun4i\eFex\* eFex\
)



if %platform%==linux goto pack_linux

echo pack_crane

xcopy /q /e chips\$PACK_CHIP\$PACK_PLATFORM\default\* out\
xcopy /q /e chips\$PACK_CHIP\$PACK_PLATFORM\$PACK_BOARD\*.fex out\
xcopy /q /e chips\$PACK_CHIP\$PACK_PLATFORM\$PACK_BOARD\*.cfg out\ 
copy eFex\card\mbr.fex out
copy eFex\split_xxxx.fex out
copy eGon\storage_media\nand\boot0.bin out
copy eGon\storage_media\nand\boot1.bin out
copy eGon\storage_media\sdcard\boot0.bin out\card_boot0.fex
copy eGon\storage_media\sdcard\boot1.bin out\card_boot1.fex
copy wboot\bootfs.ini out


copy wboot\diskfs.fex out
xcopy /q /e wboot\bootfs\* out\bootfs\


cd out
set IMG_NAME="%platform%-%board%.img"
echo imagename = %IMG_NAME% >> image.cfg
echo "" >> image.cfg
copy  %crane_out%\root.img root.fex
copy  %crane_out%\system.img system.fex
copy  %crane_out%\recovery.img recovery.fex

..\bootimg.exe --kernel kernel --ramdisk ramdisk.img  'console=ttyS0,115200 init=/init rw  loglevel=9'  --base 0x40000000 -o kernel.fex

script_old.exe  sys_config.fex
script.exe sys_config1.fex
update_23.exe sys_config1.bin boot0.bin boot1.bin
update_23.exe sys_config1.bin card_boot0.fex card_boot1.fex SDMMC_CARD
copy sys_config1.bin bootfs\script.bin
copy sys_config1.bin bootfs\script0.bin
update_mbr.exe sys_config.bin mbr.fex
fsbuild.exe "%CD%\bootfs.ini" "%CD%\split_xxxx.fex"
FileAddSum bootfs.fex vboot.fex
FileAddSum root.fex vroot.fex
FileAddSum system.fex vsystem.fex
FileAddSum recovery.fex vrecovery.fex
compile.exe -o image.bin image.cfg
dragon.exe image.cfg 

del vboot.fex 
del vroot.fex 
del vsystem.fex 
del vrecovery.fex
cd ..
goto end_pack



:pack_linux
echo pack_linux
echo chips\%chips%\configs\%platform%\default\  chips\%chips%\%platform%\%board%\
xcopy /q /e chips\%chips%\configs\%platform%\default\*  out\
xcopy /q /e chips\%chips%\configs\%platform%\%board%\* out\

copy  chips\%chips%\eFex\card\mbr.fex out
copy  chips\%chips%\eFex\split_xxxx.fex out
copy  chips\%chips%\eGon\storage_media\nand\boot0.bin out
copy  chips\%chips%\eGon\storage_media\nand\boot1.bin out
copy  chips\%chips%\eGon\storage_media\sdcard\boot0.bin out\card_boot0.fex
copy  chips\%chips%\eGon\storage_media\sdcard\boot1.bin out\card_boot1.fex
copy  chips\%chips%\wboot\bootfs.ini out


copy  chips\%chips%\wboot\diskfs.fex out
xcopy /q /e  chips\%chips%\wboot\bootfs\* out\bootfs\

pause



copy   %linux_out%\u-boot.bin out\bootfs\linux\
copy   %linux_out%\boot.img out\boot.fex
copy  %linux_out%\rootfs.ext4 out\rootfs.fex
	
	
	
cd out
set IMG_NAME="%chips%-%platform%-%board%.img"
echo imagename = %IMG_NAME% >> image.cfg
echo "" >> image.cfg


script_old.exe  sys_config.fex
script.exe sys_config1.fex
update_23.exe sys_config1.bin boot0.bin boot1.bin
update_23.exe sys_config1.bin card_boot0.fex card_boot1.fex SDMMC_CARD
copy sys_config1.bin bootfs\script.bin
copy sys_config1.bin bootfs\script0.bin
update_mbr.exe sys_config.bin mbr.fex
fsbuild.exe "%CD%\bootfs.ini" "%CD%\split_xxxx.fex" 

	
rename bootfs.fex bootloader.fex


u_boot_env_gen.exe env.cfg env.fex



compile.exe -o image.bin image.cfg
dragon.exe image.cfg 
cd ..
move out\%chips%-%platform%-%board%.img %chips%-%platform%-%board%.img
rmdir /s /q eFex
:end_pack
echo.
echo Pack Done!!!
echo.

pause

