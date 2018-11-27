C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe --i32combined --output .\Release\%1.hex   %2
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bincombined --output .\Release\%1.bin   %2
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --vhx --8x1   --output .\Release\%1.HEX1b %2
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --vhx --32x1  --output .\Release\%1.HEX4b %2
..\bin\CC_Hex2Bin.exe .\Release\%1 .\Release\enc\%1

