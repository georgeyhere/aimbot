# utils.tcl
# Common utility procedures for build scripts

proc set_if_unset {name value} {
    if {[info exists ::env($name)] && [string length $::env($name)] != 0} {
        set $name $::env($name)
    } else {
        set $name $value
    }
}

proc print_flow_config {} {
    global WORKROOT PROJECT PRJ_DIR BUILD_DIR PRJ_NAME PART BRD_PART BD_TCL_SCRIPTS STOP_AFTER
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
}

proc open_project_if_not_open {project_name} {
    # Check if the project is already open
    if {[catch {set cur [current_project]} err]} {
        # No project currently open, need to open it
        
        # Check if WORKROOT is set
        if {![info exists ::env(WORKROOT)] || [string length $::env(WORKROOT)] == 0} {
            puts "Error: WORKROOT environment variable not set. Please source setup.sh first."
            exit 1
        }

        # Ensure we are in the correct working directory
        cd $::env(WORKROOT)

        set BUILD_DIR "$::env(WORKROOT)/lib/${project_name}_build"
        set XPR_FILE "$BUILD_DIR/$project_name.xpr"
        
        if {![file exists $XPR_FILE]} {
            puts "Error: Project file $XPR_FILE not found."
            exit 1
        }
        
        puts "Opening Vivado project: $XPR_FILE"
        open_project $XPR_FILE
    } else {
        # A project is currently open, check if it's the one we want
        if {[catch {set cur_name [get_property NAME $cur]} _] == 0} {
            if {$cur_name eq $project_name} {
                puts "Project $project_name already open."
            } else {
                puts "Warning: Project $cur_name is currently open, but $project_name was requested."
            }
        }
    }
}
