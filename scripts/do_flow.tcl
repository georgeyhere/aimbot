# do_flow.tcl
# Master flow script for Vivado project build and synthesis
# Usage: vivado -mode batch -source do_flow.tcl -tclargs <PROJECT> <STOP_AFTER> -notrace -log flow_<PROJECT>.log
set start_ms [clock milliseconds]

##########################################
# 0) Flow Initialization - variable setup
###########################################
if {![info exists ::env(WORKROOT)] || [string length $::env(WORKROOT)] == 0} {
    puts "Error: WORKROOT environment variable not set. Please source setup.sh first."
    exit 1
}
cd $::env(WORKROOT)

set utils_file "$::env(WORKROOT)/scripts/utils.tcl"
if {[file exists $utils_file]} {
    source $utils_file
} elseif {[file exists "./scripts/utils.tcl"]} {
    source "./scripts/utils.tcl"
}

set PROJECT [lindex $argv 0]
if {$PROJECT eq ""} {
    puts "Error: No project name provided."
    exit 1
}
set STOP_AFTER [lindex $argv 1]
if {$STOP_AFTER eq ""} {
    set STOP_AFTER "all"
}
set WORKROOT $::env(WORKROOT)
set PRJ_DIR "$WORKROOT/prj/$PROJECT"
set BUILD_DIR "$WORKROOT/lib/${PROJECT}_build"
set DEFINES_FILE "$PRJ_DIR/${PROJECT}_defines.tcl"
if {![file exists $DEFINES_FILE]} {
    puts "Error: Defines file $DEFINES_FILE not found."
    exit 1
}
source $DEFINES_FILE
if {![info exists PRJ_NAME]} {
    puts "Error: PRJ_NAME not defined in $DEFINES_FILE."
    exit 1
}
if {![info exists PART]} {
    puts "Error: PART not defined in $DEFINES_FILE."
    exit 1
}
if {![info exists BRD_PART]} {
    puts "Error: BRD_PART not defined in $DEFINES_FILE."
    exit 1
}
print_flow_config

# Validate STOP_AFTER value
set valid_stages {create bd wrapper xdc post synthesis impl all}
if {[lsearch -exact $valid_stages $STOP_AFTER] == -1} {
    puts "Error: Invalid STOP_AFTER value '$STOP_AFTER'. Valid: $valid_stages"
    exit 1
}

##########################################
# Run the flow
###########################################
# Note that STOP_AFTER logic is self-contained in each steps' script
source "$WORKROOT/scripts/create_project.tcl"
source "$WORKROOT/scripts/run_synthesis.tcl"
source "$WORKROOT/scripts/run_impl.tcl"

