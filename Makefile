.PHONY: default
default: sim_regr

PROJECTS = tpg_vdma
STOP_AFTER ?= all

.PHONY: help
help:
	@echo "-----------------------------------------------------------------------------"
	@echo "Usage:"
	@echo "    make project  PROJECT=[name] [STOP_AFTER=<stage>]:   Build a specific Vivado project to lib/<PROJECT>."
	@echo "    make clean:                                       Clean up... everything"
	@echo "    make help:                                        Show this help message"
	@echo ""
	@echo "Valid projects: $(PROJECTS)"
	@echo "Valid STOP_AFTER stages: create, bd, xdc, post, all"
	@echo ""
	@echo "Examples:"
	@echo "    make project PROJECT=tpg_vdma                      # Full build of tpg_vdma project"
	@echo "    make project PROJECT=tpg_vdma STOP_AFTER=create    # Create project only"
	@echo "    make project PROJECT=tpg_vdma STOP_AFTER=bd        # Stop after block design recreation"
	@echo "    make project PROJECT=tpg_vdma STOP_AFTER=xdc       # Stop after adding constraints"
	@echo "    make clean                                         # Clean all build artifacts"
	@echo "-----------------------------------------------------------------------------"

#-----------------------#
# TPG VDMA Demo Project #
#-----------------------#
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
	. ./setup.sh
	mkdir -p lib
	$(XILINX_VIVADO)/bin/vivado -mode batch -source $(WORKROOT)/scripts/build_project.tcl -tclargs $(PROJECT) $(STOP_AFTER) -log vivado_$(PROJECT).log -journal vivado_$(PROJECT).jou

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


