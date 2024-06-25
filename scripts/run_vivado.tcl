# run_vivado.tcl
#   Usage: run_vivado.tcl <PROTO_PATH> <PRJ_SCRIPT> <PRJ_PART>

if {$argc != 3} {
    puts "Usage: run_vivado.tcl <PROTO_PATH> <PRJ_SCRIPT> <PRJ_PART>"
    puts $argc
    puts $argv
    exit 1
}



#set PROTO_PATH $::env(PROTO_PATH)
#set PRJ_SCRIPT $::env(PRJ_SCRIPT)
set PROTO_PATH [lindex $argv 0]
set PRJ_SCRIPT [lindex $argv 1]
set PRJ_PART   [lindex $argv 2]

set PRJ_NAME   [file rootname $PRJ_SCRIPT]
set PRJ_PATH   $PROTO_PATH/lib/$PRJ_NAME

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
#set_if_unset PRJ_PART xc7z020clg400-1
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

# 
set PRJ_SRCS $PRJ_PATH/$PRJ_NAME.srcs
set PRJ_GEN  $PRJ_PATH/$PRJ_NAME.gen
puts "----------------------"
puts "PRJ_SRCS: $PRJ_SRCS"
puts "PRJ_GEN : $PRJ_GEN"
puts "----------------------"

# Create project block diagram
source $PROTO_PATH/prj/$PRJ_SCRIPT


# Create HDL wrapper
# fixme: block diagram name must be same as project name
make_wrapper -files [get_files $PRJ_SRCS/sources_1/bd/$PRJ_NAME/$PRJ_NAME.bd] -top
add_files -norecurse $PRJ_GEN/sources_1/bd/$PRJ_NAME/hdl/${PRJ_NAME}_wrapper.v

#
generate_target all [get_files $PRJ_SRCS/sources_1/bd/$PRJ_NAME/$PRJ_NAME.bd]


