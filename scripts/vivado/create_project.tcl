# create_project.tcl
source "$::env(WORKROOT)/scripts/vivado/utils.tcl"

#############################
 # Build Configuration
#############################
print_flow_config

if {[file exists $BUILD_DIR]} {
    puts "Error: Build directory $BUILD_DIR already exists. Please remove it or choose a different project name."
    exit 1
}

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
            # Set origin_dir_loc so the BD TCL script creates the block design in the intended location
            set ::origin_dir_loc "$BUILD_DIR/prj"
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
}

puts "###########################################"
puts " 3) 'wrapper' stage"
puts " Create HDL wrapper for block designs"
puts "###########################################"
set bds [get_bd_designs -quiet]
if {[llength $bds] > 0} {
    foreach bd $bds {
        set bd_file "$BUILD_DIR/prj/${bd}/${bd}.bd"
        if {[file exists $bd_file]} {
            puts "Creating HDL wrapper for block design $bd"
            make_wrapper -files [get_files $bd_file] -top
            add_files -norecurse $BUILD_DIR/prj/${bd}/hdl/${bd}_wrapper.v
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
}

puts "######################################"
puts " 5) 'post' stage"
puts " Optional post-flow script"
puts "#######################################"
set POST_FLOW_FILE "$PRJ_DIR/${PRJ_NAME}_post_create_project.tcl"
if {[file exists $POST_FLOW_FILE]} {
    puts "Sourcing post-create-project script $POST_FLOW_FILE"
    source $POST_FLOW_FILE
    puts "Post-create-project script executed successfully."
} else {
    puts "No post-create-project script found at $POST_FLOW_FILE."
}

if {$STOP_AFTER eq "post"} {
    puts "Stopping after post-flow script."
    exit 0
} 

puts "Project Creation completed."
set end_ms [clock milliseconds]
set runtime_ms [expr {$end_ms - $start_ms}]
set total_seconds [expr {$runtime_ms / 1000.0}]
set minutes [expr {int($total_seconds / 60)}]
set seconds [expr {$total_seconds - ($minutes * 60)}]
puts "Project Creation Time: ${minutes} minute(s) and [format "%.2f" $seconds] second(s)"
