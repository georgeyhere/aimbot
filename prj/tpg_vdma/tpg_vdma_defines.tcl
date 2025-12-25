# Vivado project settings for VDMA TPG project

set PRJ_NAME "vdma_tpg"
set PART     "xc7z020clg400-1"
set BRD_PART "digilentinc.com:zybo-z7-20:part0:1.2"

# Block design TCL scripts to be run during build_project. 
# These should be generated from Vivado via write_bd_tcl. 
# For example:
# > write_bd_tcl -force /home/george/repos/aimbot/prj/tpg_vdma/bd/vdma_tpg.tcl
set BD_TCL_SCRIPTS [list "bd/vdma_tpg_bd.tcl"]

