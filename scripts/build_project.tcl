# build_project.tcl 


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

# Set board repository path to include Digilent boards
set_param board.repoPaths [list $WORKROOT/digilent/vivado-boards]

# Set IP repository paths to include Digilent IP library
set_param ip_repo_paths [list $WORKROOT/digilent/vivado-library]

# Check if build directory already exists
if {[file exists $BUILD_DIR]} {
    puts "Error: Build directory $BUILD_DIR already exists. Please remove it or choose a different project name."
    exit 1
}

#############################
# 1) "create" stage
# Create the Vivado project
#############################
puts "Creating project $PRJ_NAME in $BUILD_DIR"
create_project $PRJ_NAME $BUILD_DIR -part $PART -force
set_property board_part $BRD_PART [current_project]

if {$STOP_AFTER eq "create"} {
    puts "Stopping after project creation."
    exit 0
}

###########################################
# 2) "bd" stage
# Recreate block diagram (if there is one)
###########################################
set BD_FILE "$PRJ_DIR/${PRJ_NAME}.bd"
if {[file exists $BD_FILE]} {
    puts "Adding and opening block design $BD_FILE"
    add_files $BD_FILE
    open_bd_design $BD_FILE
    regenerate_bd_layout
    save_bd_design
    close_bd_design [get_bd_designs]
} else {
    puts "Warning: Block design file $BD_FILE not found."
}

if {$STOP_AFTER eq "bd"} {
    puts "Stopping after block design recreation."
    exit 0
}

######################################
# 3) "xdc" stage
# Add XDC constraints if applicable
######################################
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
}

######################################
# 4) "post" stage
# Optional post-flow script
#######################################
set POST_FLOW_FILE "$PRJ_DIR/${PRJ_NAME}_post_flow.tcl"
if {[file exists $POST_FLOW_FILE]} {
    puts "Sourcing post-flow script $POST_FLOW_FILE"
    source $POST_FLOW_FILE
} else {
    puts "No post-flow script found at $POST_FLOW_FILE."
}

if {$STOP_AFTER eq "post"} {
    puts "Stopping after post-flow script."
    exit 0
}

puts "Project build completed."