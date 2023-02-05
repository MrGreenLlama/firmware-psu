@echo off
set BAUD=115200
set config=wiring
set comPort=
set /p comPort=Specify the COM port on which the controller is connected to: 
set cpu=
set /p cpu=Specify the CPU for compiling (e.g.: ATmega2560): 
if %cpu% EQU atmega328p (echo prompting for old or new bootloader.....) else (goto skip)
set old=
set /p old=Old bootloader (please type YES or NO (in capital letters))?: 
if %old% EQU YES (set config=arduino & set BAUD=57600 & goto skip)
if %old% EQU NO (goto skip)
:skip
echo downloading latest firmware.....cd 7z
cd 7z
del *.xz > nul
curl https://github.com/izakc-spc/firmware-psu/raw/main/release/firmware.hex.xz -L -o firmware.hex.xz
if Errorlevel = 1 (echo firmware couldn't be downloaded, server unreachable & echo press any key to exit & pause > nul) else (echo downloaded firmware, trying to install....)
7z e firmware.hex.xz
cd..
move 7z\firmware.hex avrdude\fw\
del 7z\firmware.hex.xz > nul
cd avrdude
avrdude -C avrdude.conf -v -p%cpu% -c%config% -P%comPort% -b%BAUD% -D -Uflash:w:fw\firmware.hex:i
echo press any key to exit...
del fw\*.hex > nul
pause > nul