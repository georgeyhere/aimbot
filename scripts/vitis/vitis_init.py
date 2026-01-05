# vitis_init.py
#
# References:
# https://github.com/Xilinx/Vitis-Tutorials/blob/2025.2/Embedded_Software/Getting_Started/scripts/build_workspace.py
# https://docs.amd.com/r/en-US/ug1400-vitis-embedded
# https://adaptivesupport.amd.com/s/question/0D54U00008rVUy9SAG/vitis-unified-ide-20242-import-filessources-as-soft-links?language=en_US
#
# Usage:
# python3 scripts/vitis/vitis_init.py --prj <project_name>
#
import os
import argparse
import yaml

import sys
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'vitis'))
import vitis_utils
import vitis

def main():
    
    parser = argparse.ArgumentParser(
        description="Vitis Project Initialization Script"
    )
    parser.add_argument(
        "--prj",
        type=str,
        required=True,
        help="Project name, i.e. tpg_vdma"
    )
    args = parser.parse_args()

    # Check for the project-specific YAML file
    # $WORKROOT/prj/<prj>/<prj>.yaml
    WORKROOT = os.environ.get("WORKROOT", os.getcwd())
    prj = args.prj
    yaml_path = os.path.join(WORKROOT, "prj", prj, f"{prj}.yaml")
    if not os.path.isfile(yaml_path):
        print(f"YAML file not found: {yaml_path}")
        return
    
    with open(yaml_path, 'r') as f:
        config = yaml.safe_load(f)
        
    # Create the Vitis workspace directory
    # By default, use $WORKROOT/lib/<prj>_vitis
    # For example - $WORKROOT/lib/tpg_vdma_vitis
    workspace_path = os.path.join(WORKROOT, "lib", f"{prj}_vitis")
    if not os.path.exists(workspace_path):
        os.makedirs(workspace_path)
        print(f"Vitis workspace directory created: {workspace_path}")
    else:
        print(f"WARNING: Vitis workspace directory already exists: {workspace_path}")

    # Set up the Vitis client    
    client = vitis.create_client()
    client.set_workspace(path=workspace_path)

    # Print the YAML config
    print("------------------------------------")
    print(f"YAML config file: {yaml_path}")
    print(yaml.dump(config, sort_keys=False))
    print("------------------------------------")

    # Create the hardware platform from xsa
    platform = client.create_platform_component(
        name=config['hw_platform']['platform_name'],
        hw_design=config['hw_platform']['hw_design'],
        cpu=config['hw_platform']['cpu'],
        os=config['hw_platform']['os'],
        domain_name=config['hw_platform']['domain_name']
    )

    # Find the .xpfm file in the workspace_path
    # This logic is a bit hacky. 
    # But there should only be 1 xpfm per platform component...
    # and only 1 platform component per project.
    # TODO: clean this up
    xpfm = None
    for root, dirs, files in os.walk(workspace_path):
        for file in files:
            if file.endswith('.xpfm'):
                xpfm = os.path.join(root, file)
                break  
        if xpfm:
            break
    if not xpfm:
        print(f"ERROR: No .xpfm file found in workspace: {workspace_path}")
        client.close()
        return

    # Create application components for each app in the YAML
    for key, app in config.items():
        if not key.startswith('app_'):
            continue
        app_name = app.get('app_name', key)
        print(f"Creating app component: {app_name}")
        comp = client.create_app_component(
            name=app_name,
            platform=xpfm,
            domain=config['hw_platform']['domain_name']
        )

        # Import sources for each app if 'import' is specified in YAML
        import_path = app.get('import')
        if import_path:
            src_dir = os.path.join(workspace_path, app_name, "src")
            print(f"Symlinking files from {import_path} to {src_dir}")
            vitis_utils.symlink_app_sources(import_path, src_dir, app_name=app_name)

    platform.build()
    

    # Close the Vitis client
    client.close()

if __name__ == "__main__":
    main()