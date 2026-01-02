# 2026-01-01T17:22:30.081007358
import vitis

client = vitis.create_client()
client.set_workspace(path="vitis")

platform = client.get_component(name="tpg_vdma_platform")
status = platform.build()

comp = client.get_component(name="app_tpg_vdma_demo")
comp.build()

