.PHONY: default
default: sim_regr

PROJECTS = tpg_vdma
STOP_AFTER ?= all

# Runtime options
GUI ?= 0

VIVADO_MODE = batch
ifeq ($(GUI),1)
VIVADO_MODE = gui
endif

.PHONY: help
help:
	@echo "-----------------------------------------------------------------------------"
	@echo "Usage:"
	@echo "    make project  PROJECT=[name] [STOP_AFTER=<stage>] [GUI=1]:   Build a specific Vivado project to lib/<PROJECT>."
	@echo "    make clean:                                       Clean up... everything"
	@echo "    make help:                                        Show this help message"
	@echo ""
	@echo "Valid projects: $(PROJECTS)"
	@echo "Valid STOP_AFTER stages: create, bd, wrapper, xdc, post, synthesis, all"
	@echo ""
	@echo "Examples:"
	@echo "    make project PROJECT=tpg_vdma                      # Full build of tpg_vdma project"
	@echo "    make project PROJECT=tpg_vdma STOP_AFTER=create    # Create project only"
	@echo "    make project PROJECT=tpg_vdma STOP_AFTER=bd        # Stop after block design recreation"
	@echo "    make project PROJECT=tpg_vdma STOP_AFTER=wrapper   # Stop after HDL wrapper creation"
	@echo "    make project PROJECT=tpg_vdma STOP_AFTER=xdc       # Stop after adding constraints"
	@echo "    make project PROJECT=tpg_vdma STOP_AFTER=synthesis # Run synthesis after build"
	@echo ""
	@echo "GUI Options:"
	@echo "    make project PROJECT=tpg_vdma GUI=1                # Full build in Vivado GUI"
	@echo ""
	@echo "Other:"
	@echo "    make clean                                         # Clean all build artifacts"
	@echo "-----------------------------------------------------------------------------"


.PHONY: project
project:
	@if [ -z "$(PROJECT)" ]; then \
		echo "Error: PROJECT variable not set. Use make project PROJECT=<name>"; \
		exit 1; \
	fi
	@if ! echo "$(PROJECTS)" | grep -q "^$(PROJECT)$$"; then \
		echo "Error: Invalid project '$(PROJECT)'. Valid projects: $(PROJECTS)"; \
		exit 1; \
	fi
	@if [ -z "$$WORKROOT" ]; then \
		echo "Error: WORKROOT not set. Please source ./setup.sh first."; \
		exit 1; \
	fi
	mkdir -p lib
	$(XILINX_VIVADO)/bin/vivado -mode $(VIVADO_MODE) -source $(WORKROOT)/scripts/do_flow.tcl -tclargs $(PROJECT) $(STOP_AFTER) -notrace -log flow_$(PROJECT).log

.PHONY: clean
clean:
	rm -rf .Xil
	rm -rf xvlog*
	rm -rf xelab*
	rm -rf xsim*
	rm -rf *.log
	rm -rf *wdb
	rm -rf *.jou
	rm -rf *.str 
	rm -rf transcript
	rm -rf work
	rm -rf lib


