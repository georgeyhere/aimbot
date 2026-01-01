# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/diskio.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/ff.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/ffconf.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/sleep.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/xilffs.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/xilffs_config.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/xilrsa.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/xiltimer.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/include/xtimer_config.h"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/lib/libxilffs.a"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/lib/libxilrsa.a"
  "/home/george/repos/aimbot/prj/tpg_vdma/vitis/tpg_vdma_platform/zynq_fsbl/zynq_fsbl_bsp/lib/libxiltimer.a"
  )
endif()
