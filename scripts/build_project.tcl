# build_project.tcl 
set start_ms [clock milliseconds]

# Check if WORKROOT is set
if {![info exists ::env(WORKROOT)] || [string length $::env(WORKROOT)] == 0} {
    puts "Error: WORKROOT environment variable not set. Please source setup.sh first."
    exit 1
}

# Ensure we are in the correct working directory
cd $::env(WORKROOT)

# Get project name from arguments
set PROJECT [lindex $argv 0]
if {$PROJECT eq ""} {
    puts "Error: No project name provided."
    exit 1
}

set STOP_AFTER [lindex $argv 1]
if {$STOP_AFTER eq ""} {
    set STOP_AFTER "all"
}

########################
# Setup project paths
########################
set WORKROOT $::env(WORKROOT)

# PRJ_DIR: Directory containing project sources
set PRJ_DIR "$WORKROOT/prj/$PROJECT"

# BUILD_DIR: Directory to create the Vivado project
set BUILD_DIR "$WORKROOT/lib/${PROJECT}_build"

# Each project should have its own defines file.
# This file contains project-specific variables. 
# At a minimum, it should define PRJ_NAME, PART, and BRD_PART.
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

# Check if build directory already exists
if {[file exists $BUILD_DIR]} {
    puts "Error: Build directory $BUILD_DIR already exists. Please remove it or choose a different project name."
    exit 1
}

#############################
# Build Configuration
#############################
puts "####################################################"
puts "Build Configuration:"
puts "WORKROOT     : $WORKROOT"
puts "PROJECT      : $PROJECT"
puts "PRJ_DIR      : $PRJ_DIR"
puts "BUILD_DIR    : $BUILD_DIR"
puts "PRJ_NAME     : $PRJ_NAME"
puts "PART         : $PART"
puts "BRD_PART     : $BRD_PART"
if {[info exists BD_TCL_SCRIPTS]} {
    puts "BD_TCL_SCRIPTS: $BD_TCL_SCRIPTS"
} else {
    puts "BD_TCL_SCRIPTS: (none)"
}
puts "STOP_AFTER   : $STOP_AFTER"
puts "####################################################"

puts "############################"
puts " 1) 'create' stage"
puts " Create the Vivado project"
puts "############################"
puts "Creating project $PRJ_NAME in $BUILD_DIR targeting $BRD_PART"

# Set board repository path
set board_repo_path [file normalize "$WORKROOT/digilent/vivado-boards"]
puts "Board repository path: $board_repo_path"
set_param board.repoPaths [list $board_repo_path]

# Create the project
create_project $PRJ_NAME $BUILD_DIR -part $PART -force
set_property BOARD_PART $BRD_PART [current_project]

# Add Digilent IP repo by default
# TODO: update flow to set IP library dependencies in project config
set_property ip_repo_paths [list $WORKROOT/digilent/vivado-library] [current_project]
update_ip_catalog

if {$STOP_AFTER eq "create"} {
    puts "Stopping after project creation."
    exit 0
} else {
    puts "Project $PRJ_NAME created successfully."
}

puts "##########################################"
puts " 2) 'bd' stage"
puts " Recreate block diagram (if there is one)"
puts "##########################################"
cd $BUILD_DIR
if {[llength $BD_TCL_SCRIPTS] > 0} {
    foreach script $BD_TCL_SCRIPTS {
        set script_path "$PRJ_DIR/$script"
        if {[file exists $script_path]} {
            puts "Sourcing BD TCL script $script_path"
            source $script_path
        } else {
            puts "Warning: BD TCL script $script_path not found."
        }
    }
    puts "Block design TCL scripts processed successfully."
} else {
    puts "No BD TCL scripts defined."
}

if {$STOP_AFTER eq "bd"} {
    puts "Stopping after block design recreation."
    exit 0
} else {
    puts "Block design TCL scripts processed successfully."
}

puts "###########################################"
puts " 3) 'wrapper' stage"
puts " Create HDL wrapper for block designs"
puts "###########################################"
set bds [get_bd_designs -quiet]
if {[llength $bds] > 0} {
    foreach bd $bds {
        set bd_file "$BUILD_DIR/${PRJ_NAME}.srcs/sources_1/bd/${bd}/${bd}.bd"
        if {[file exists $bd_file]} {
            puts "Creating HDL wrapper for block design $bd"
            make_wrapper -files [get_files $bd_file] -top
        } else {
            puts "Warning: BD file $bd_file not found for wrapper creation."
        }
    }
    puts "HDL wrappers created successfully."
} else {
    puts "No block designs found for wrapper creation."
}

if {$STOP_AFTER eq "wrapper"} {
    puts "Stopping after HDL wrapper creation."
    exit 0
} else {
    puts "HDL wrappers created successfully."
}

puts "######################################"
puts " 4) 'xdc' stage"
puts " Add XDC constraints if applicable"
puts "######################################"
set XDC_FILES [glob -nocomplain $WORKROOT/prj/$PROJECT/constr/*.xdc]
if {[llength $XDC_FILES] > 0} {
    puts "Adding XDC files: $XDC_FILES"
    add_files -fileset constrs_1 $XDC_FILES
} else {
    puts "No XDC files found in constr/."
}

if {$STOP_AFTER eq "xdc"} {
    puts "Stopping after adding XDC constraints."
    exit 0
} else {
    puts "XDC files added successfully."
}

puts "######################################"
puts " 5) 'post' stage"
puts " Optional post-flow script"
puts "#######################################"
set POST_FLOW_FILE "$PRJ_DIR/${PRJ_NAME}_post_flow.tcl"
if {[file exists $POST_FLOW_FILE]} {
    puts "Sourcing post-flow script $POST_FLOW_FILE"
    source $POST_FLOW_FILE
    puts "Post-flow script executed successfully."
} else {
    puts "No post-flow script found at $POST_FLOW_FILE."
}

if {$STOP_AFTER eq "post"} {
    puts "Stopping after post-flow script."
    exit 0
} 

puts "Project build completed."
set end_ms [clock milliseconds]
set runtime_ms [expr {$end_ms - $start_ms}]
set total_seconds [expr {$runtime_ms / 1000.0}]
set minutes [expr {int($total_seconds / 60)}]
set seconds [expr {$total_seconds - ($minutes * 60)}]
puts "Total runtime: ${minutes} minute(s) and [format "%.2f" $seconds] second(s)"