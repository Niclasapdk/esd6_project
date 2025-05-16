# Building

[https://e2e.ti.com/support/processors-group/processors/f/processors-forum/590926/ccs-tms320c5517-again-how-to-burn-program-into-flash-on-5517](https://e2e.ti.com/support/processors-group/processors/f/processors-forum/590926/ccs-tms320c5517-again-how-to-burn-program-into-flash-on-5517)
[https://e2e.ti.com/support/processors-group/processors/f/processors-forum/1363364/tms320c5535-request-for-updated-link-to-c5535-ezdsp-programmer-utility](https://e2e.ti.com/support/processors-group/processors/f/processors-forum/1363364/tms320c5535-request-for-updated-link-to-c5535-ezdsp-programmer-utility)

Creation of bootable image is described in section 3.1.1.1 of [C5535 Bootloader guide](https://www.ti.com/lit/an/sprabl7d/sprabl7d.pdf?ts=1747316903870&ref_url=https%253A%252F%252Fwww.google.com%252F).
Create hex image by:
```
# general
${CG_TOOL_ROOT}/hex55.exe -boot -v5505 -serial8 -b -o bootimg.bin Release/beep_boop.out
# for most installations:
& "C:\Program Files (x86)\Texas Instruments\ccsv4\tools\compiler\c5500\bin\hex55.exe" -boot -v5505 -serial8 -b -o bootimg.bin Release/beep_boop.out
```

Then (from CCS) launch TI Debugger, Connect to target and load program `programmer_C5535_eZdsp.out`, which is installed from [C55XCSL-LOWPOWER](https://www.ti.com/tool/SPRC133).
Afterwards the programmer can be found in `C:\ti\c55_lp\c55_csl_3.08.01\utilities\Programmer\c5535`.

Follow the programmer's instructions in the console:
```
C5535 eZDSP...
Input <filepath>
C:\Users\marku\uni\esd6_project\implementation\bootimg.bin
SPI Flash...
Erasing chip (SPI Flash)...
Opening C:\Users\marku\uni\esd6_project\implementation\bootimg.bin...
Input file opened
Programming Complete
```

After the program exits (processor is halted and in function `abort()` from `exit.c`), it is safe to close the debugger.
