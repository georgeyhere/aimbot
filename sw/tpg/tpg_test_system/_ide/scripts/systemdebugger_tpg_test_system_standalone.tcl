# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\george\Documents\GitHub\aimbot\sw\tpg\tpg_test_system\_ide\scripts\systemdebugger_tpg_test_system_standalone.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\george\Documents\GitHub\aimbot\sw\tpg\tpg_test_system\_ide\scripts\systemdebugger_tpg_test_system_standalone.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A" && level==0 && jtag_device_ctx=="jsn-Zybo Z7-210351B04BB2A-23727093-0"}
fpga -file C:/Users/george/Documents/GitHub/aimbot/sw/tpg/tpg_test/_ide/bitstream/tpg_sys_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/george/Documents/GitHub/aimbot/sw/tpg/tpg_sys_wrapper/export/tpg_sys_wrapper/hw/tpg_sys_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/george/Documents/GitHub/aimbot/sw/tpg/tpg_test/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Users/george/Documents/GitHub/aimbot/sw/tpg/tpg_test/Debug/tpg_test.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
