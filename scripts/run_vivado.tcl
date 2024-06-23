# run_vivado.tcl
#   Usage: run_vivado.tcl <PROTO_PATH> <PRJ_SCRIPT> <PRJ_PART>

#if {$argc != 3} {
#    puts "Usage: run_vivado.tcl <PROTO_PATH> <PRJ_SCRIPT> <PRJ_PART>"
#    exit 1
#}

set PROTO_PATH $::env(PROTO_PATH)
set PRJ_SCRIPT $::env(PRJ_SCRIPT)

set PRJ_NAME   [file rootname $PRJ_SCRIPT]
set PRJ_PATH   $PROTO_PATH/lib/$PRJ_NAME

set PRJ_PART ""
set BRD_PART ""

# Optional vars
proc set_if_unset {name value} {
    if {[info exists ::env($name)] && [expr {$::env($name) ne ""}]} {
        set $name $::env($name)
        puts "here"
    } else {
        set $name $value
        puts "Here1"
    }
}



#set_if_unset PRJ_PART "xc7z020clg400-1"
#set_if_unset BRD_PART "digilentinc.com:zybo-z7-20:part0:1.1"
set PRJ_PART xc7z020clg400-1
#set BRD_PART digilentinc.com:zybo-z7-20:part0:1.1

puts ""
puts "------------------------------"
puts "run_vivado.tcl"
puts ""
puts "PRJ_NAME:   $PRJ_NAME"
puts "PRJ_PATH:   $PRJ_PATH"
puts "PROTO_PATH: $PROTO_PATH"
puts "PRJ_PART:   $PRJ_PART"
puts "PRJ_SCRIPT: $PRJ_SCRIPT"
puts ""
#puts "BRD_PART:   $BRD_PART"
puts "------------------------------"

# Create the project
create_project $PRJ_NAME $PRJ_PATH -part $PRJ_PART
#set_property board_part $BRD_PART [current_project]
set_property ip_repo_paths $PROTO_PATH/ip [current_project]
update_ip_catalog

# Source project script
source $PROTO_PATH/prj/$PRJ_SCRIPT


