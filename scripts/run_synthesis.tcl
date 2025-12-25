# run_synthesis.tcl
# Usage: vivado -mode batch -source run_synthesis.tcl -notrace -log synth.log

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

set BUILD_DIR "$::env(WORKROOT)/lib/${PROJECT}_build"

# Open the project
set PRJ_NAME $PROJECT
set XPR_FILE "$BUILD_DIR/$PRJ_NAME.xpr"
if {![file exists $XPR_FILE]} {
    puts "Error: Project file $XPR_FILE not found."
    exit 1
}

# Check if the project is already open. If so, skip open_project.
set already_open 0
if {[catch {set cur [current_project]} err]} {
    set already_open 0
} else {
    # current_project returned a project handle; check its name
    if {[catch {set cur_name [get_property NAME $cur]} _] == 0} {
        if {$cur_name eq $PRJ_NAME} {
            puts "Project $PRJ_NAME already open (current project); skipping open_project."
            set already_open 1
        }
    }
}

if {$already_open == 0} {
    puts "Opening Vivado project: $XPR_FILE"
    open_project $XPR_FILE
}


puts "Launching synthesis: launch_runs synth_1 -jobs 16"
set rc [catch {launch_runs synth_1 -jobs 16} synth_result]
if {$rc != 0} {
    puts "Error launching synthesis: $synth_result"
    exit $rc
} else {
    puts "Synthesis launched successfully (runs started). Waiting for completion..."
    set rc2 [catch {wait_on_run synth_1 -quiet} wait_result]
    if {$rc2 != 0} {
        puts "Error while waiting for synthesis run: $wait_result"
        exit $rc2
    } else {
        puts "Synthesis run completed successfully."
    }
}

if {$STOP_AFTER eq "synthesis" || $STOP_AFTER eq "synth"} {
    puts "Stopping after synthesis."
    exit 0
}