# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/george/repos/aimbot/sw/cam_vdma/vdma_sys_wrapper/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/george/repos/aimbot/sw/cam_vdma/vdma_sys_wrapper/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {vdma_sys_wrapper}\
-hw {/home/george/repos/aimbot/sw/cam_vdma/vdma_sys_wrapper.xsa}\
-out {/home/george/repos/aimbot/sw/cam_vdma}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {vdma_sys_wrapper}
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -quick
platform config -updatehw {/home/george/repos/aimbot/sw/cam_vdma/vdma_sys_wrapper.xsa}
platform generate
platform config -updatehw {/home/george/repos/aimbot/sw/cam_vdma/vdma_sys_wrapper.xsa}
platform generate -domains 
platform active {vdma_sys_wrapper}
platform config -updatehw {/home/george/repos/aimbot/sw/cam_vdma/vdma_sys_wrapper.xsa}
platform clean
platform generate
platform generate
platform generate
