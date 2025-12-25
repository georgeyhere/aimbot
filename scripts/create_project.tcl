# shim for backward compatibility
puts "Warning: build_project.tcl is deprecated; using create_project.tcl instead."
if {[file exists "$::env(WORKROOT)/scripts/create_project.tcl"]} {
    source "$::env(WORKROOT)/scripts/create_project.tcl"
} else {
    puts "Error: create_project.tcl not found in $::env(WORKROOT)/scripts."
    exit 1
}
