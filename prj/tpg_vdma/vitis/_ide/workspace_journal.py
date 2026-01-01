# 2026-01-01T15:16:01.208323023
import vitis

client = vitis.create_client()
client.set_workspace(path="vitis")

platform = client.create_platform_component(name = "tpg_vdma_platform",hw_design = "$COMPONENT_LOCATION/../../tpg_vdma.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",compiler = "gcc")

