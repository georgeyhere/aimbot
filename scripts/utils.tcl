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
