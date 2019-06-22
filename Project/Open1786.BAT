SET PATH=C:\Keil\ARM\ARMCC\bin\;C:\Program Files (x86)\Razer Chroma SDK\bin;C:\Program Files\Razer Chroma SDK\bin;C:\Program Files (x86)\Razer\ChromaBroadcast\bin;C:\Program Files\Razer\ChromaBroadcast\bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Windows\system32\config\systemprofile\AppData\Local\Microsoft\WindowsApps;C:\Users\Nicolas Sueur\AppData\Local\Microsoft\WindowsApps;;C:\Users\Nicolas Sueur\AppData\Local\Programs\Microsoft VS Code\bin
SET CPU_TYPE=LPC1768
SET CPU_VENDOR=NXP (founded by Philips)
SET UV2_TARGET=Open1786
SET CPU_CLOCK=0x00B71B00
"C:\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\output\startup_lpc17xx._ia"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\system_lpc17xx.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\core_cm3.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_clkpwr.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_gpio.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_pinsel.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_timer.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_exti.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_nvic.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_ssp.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_i2c.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\main.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\ili_lcd_general.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lcd_api.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\english_16x8.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\affichagelcd.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\touch_panel.__i"
"C:\Keil\ARM\ARMCC\bin\ArmLink" --Via ".\Output\Project.lnp"
"C:\Keil\ARM\ARMCC\bin\fromelf.exe" ".\Output\Project.axf" --i32combined --output ".\Output\Project.hex"
