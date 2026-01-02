# run_synthesis.tcl
# Run synthesis for a Vivado project
# Usage (standalone): vivado -mode batch -source run_synthesis.tcl -tclargs <PROJECT> -notrace -log synth.log
# Usage (sourced):    Called from do_flow.tcl with PROJECT variable already defined

# Setup
if {![info exists ::env(WORKROOT)] || [string length $::env(WORKROOT)] == 0} {
    puts "Error: WORKROOT environment variable not set. Please source setup.sh first."
    exit 1
}
source $::env(WORKROOT)/scripts/vivado/utils.tcl

# Make sure the Vivado project is opened.
# PROJECT variable is already set when run from do_flow.tcl.
# However, if it's run standalone, it needs to be passed in as an argument.
if {![info exists PROJECT] || $PROJECT eq ""} {
    set PROJECT [lindex $argv 0]
    if {$PROJECT eq ""} {
        puts "Error: No project name provided."
        exit 1
    }
}
open_project_if_not_open $PROJECT

# Apply synthesis options if defined in the project defines file
if {[info exists SYNTH_OPTS] && [llength $SYNTH_OPTS] > 0} {
    puts "Applying synthesis options to synth_1 run:"
    foreach {key value} $SYNTH_OPTS {
        puts "  Setting STEPS.SYNTH_DESIGN.ARGS.$key = $value"
        set_property STEPS.SYNTH_DESIGN.ARGS.$key $value [get_runs synth_1]
    }
}

# Launch the synthesis  
# TODO: Let user specify number of jobs via setup.sh
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