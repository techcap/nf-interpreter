@echo off
"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -c UR SWD -ME -V "while_programming" -p "build\nanoBooter.hex" -p "build\nanoClr.hex" -Rst
pause