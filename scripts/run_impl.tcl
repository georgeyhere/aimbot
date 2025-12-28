# run_impl.tcl
# Run implementation for a Vivado project
# Usage (standalone): vivado -mode batch -source run_impl.tcl -tclargs <PROJECT> -notrace -log impl.log
# Usage (sourced):    Called from do_flow.tcl with PROJECT variable already defined

# Setup
if {![info exists ::env(WORKROOT)] || [string length $::env(WORKROOT)] == 0} {
    puts "Error: WORKROOT environment variable not set. Please source setup.sh first."
    exit 1
}
source $::env(WORKROOT)/scripts/utils.tcl

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

# Apply implementation options if defined in the project defines file

# Enable/disable physical optimization
if {[info exists PHYS_OPT_DESIGN_EN] && $PHYS_OPT_DESIGN_EN ne ""} {
    puts "Setting STEPS.PHYS_OPT_DESIGN.IS_ENABLED = $PHYS_OPT_DESIGN_EN"
    set_property STEPS.PHYS_OPT_DESIGN.IS_ENABLED $PHYS_OPT_DESIGN_EN [get_runs impl_1]
}

# Apply physical optimization options
if {[info exists PHYS_OPT_OPTS] && [llength $PHYS_OPT_OPTS] > 0} {
    puts "Applying physical optimization options to impl_1 run:"
    foreach {key value} $PHYS_OPT_OPTS {
        puts "  Setting STEPS.PHYS_OPT_DESIGN.ARGS.$key = $value"
        set_property STEPS.PHYS_OPT_DESIGN.ARGS.$key $value [get_runs impl_1]
    }
}

# Apply optimization options
if {[info exists OPT_OPTS] && [llength $OPT_OPTS] > 0} {
    puts "Applying optimization options to impl_1 run:"
    foreach {key value} $OPT_OPTS {
        puts "  Setting STEPS.OPT_DESIGN.ARGS.$key = $value"
        set_property STEPS.OPT_DESIGN.ARGS.$key $value [get_runs impl_1]
    }
}

# Apply place options
if {[info exists PLACE_OPTS] && [llength $PLACE_OPTS] > 0} {
    puts "Applying place options to impl_1 run:"
    foreach {key value} $PLACE_OPTS {
        puts "  Setting STEPS.PLACE_DESIGN.ARGS.$key = $value"
        set_property STEPS.PLACE_DESIGN.ARGS.$key $value [get_runs impl_1]
    }
}

# Apply route options
if {[info exists ROUTE_OPTS] && [llength $ROUTE_OPTS] > 0} {
    puts "Applying route options to impl_1 run:"
    foreach {key value} $ROUTE_OPTS {
        puts "  Setting STEPS.ROUTE_DESIGN.ARGS.$key = $value"
        set_property STEPS.ROUTE_DESIGN.ARGS.$key $value [get_runs impl_1]
    }
}

# Launch implementation
# TODO: Let user specify number of jobs via setup.sh
puts "Launching implementation: launch_runs impl_1 -jobs 16"
set rc [catch {launch_runs impl_1 -jobs 16} impl_result]
if {$rc != 0} {
    puts "Error launching implementation: $impl_result"
    exit $rc
} else {
    puts "Implementation launched successfully (runs started). Waiting for completion..."
    set rc2 [catch {wait_on_run impl_1 -quiet} wait_result]
    if {$rc2 != 0} {
        puts "Error while waiting for implementation run: $wait_result"
        exit $rc2
    } else {
        puts "Implementation run completed successfully."
    }
}

if {$STOP_AFTER eq "impl"} {
    puts "Stopping after implementation."
    exit 0
}
