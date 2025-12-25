

# aimbot

Under construction ...

## Cloning

This repository uses git submodules. To clone with dependencies:

```bash
git clone --recursive <repository-url>
```

If you already cloned without `--recursive`, initialize submodules with:

```bash
git submodule update --init --recursive
```

## Repository Structure

## Build Flow

This repo uses a primarily Make/TCL-based flow to drive the Vivado build process.

- `make project PROJECT=<name> STOP_AFTER=<>` invokes the main Tcl flow script: `scripts/do_flow.tcl`. 

`do_flow.tcl` then performs the following:  
1) Sources `utils.tcl` 
2) Sources project specific defines from `prj/<PROJECT>/<PROJECT>_defines.tcl`
3) Executes `create_project.tcl` to create a Vivado project, setup block design, etc etc
4) Executes `run_synthesis.tcl` to kick a synthesis run.
5) Executes `run_par.tcl` to kick a placement run.
