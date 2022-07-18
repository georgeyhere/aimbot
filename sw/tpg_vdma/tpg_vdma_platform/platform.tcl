# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\george\Documents\GitHub\aimbot\sw\tpg_vdma\tpg_vdma_platform\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\george\Documents\GitHub\aimbot\sw\tpg_vdma\tpg_vdma_platform\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {tpg_vdma_platform}\
-hw {C:\Users\george\Documents\GitHub\aimbot\sw\tpg_vdma\vdma_sys_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {C:/Users/george/Documents/GitHub/aimbot/sw/tpg_vdma}

platform write
platform generate -domains 
platform active {tpg_vdma_platform}
platform generate
