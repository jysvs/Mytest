This folder contains the firmware code programmed in the microcontroller unit (MCU) for the manuscript:

"Smart facemask for wireless CO2 optical determination through flexible battery-less NFC tag" 
by P. Escobedo, M.D. Fernández-Ramos, N. López-Ruiz, O. Moyano-Rodríguez, A. Martínez Olmos, I.M. Pérez de Vargas-Sansalvador, M.A. Carvajal, L.F. Capitán-Vallvey, and A.J. Palma

System requirements
--------------------
- The developed firmware has been tested against the following:
     Compiler:  XC8 1.35
     IDE software:  MPLAB X IDE v5.45
- Hardware: MCU model PIC16LF1703
- Programming tool: PICKit 3 In-Circuit-Debugger
     Firmware Suite Version: 01.56.09
     Firmware type: Enhanced Midrange
- Required non-standard hardware: NFC tag described in the manuscript

Installation guide
-------------------
- Install MPLAB X IDE (Typical install time on a "normal" desktop computer: 15 min)
- Open "AS3955pea2021_v4" project in MPLAB X IDE
- Click on "Clean and Build Main Project"
- Connect PICKit 3 to the USB port attach to the programming interface of the PIC16LF1703 device
- Click on "Make and Program Decive Main Project"

Demo
-----
A working demo can be seen in Supplementary Movie in combination with the custom-developed smartphone application
to get the optical (RGB) and temperature measurements by means of the NFC link.

Notes
------
For questions about the code, please e-mail Pablo Escobedo (pabloescobedo@ugr.es)
