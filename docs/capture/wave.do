onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /cam_capture_maxis_tb/i_pclk
add wave -noupdate /cam_capture_maxis_tb/i_resetn
add wave -noupdate -expand -group {DUT Interface} -divider INPUTS
add wave -noupdate -expand -group {DUT Interface} /cam_capture_maxis_tb/i_vsync
add wave -noupdate -expand -group {DUT Interface} /cam_capture_maxis_tb/i_href
add wave -noupdate -expand -group {DUT Interface} -radix hexadecimal /cam_capture_maxis_tb/i_data
add wave -noupdate -expand -group {DUT Interface} -divider OUTPUTS
add wave -noupdate -expand -group {DUT Interface} /cam_capture_maxis_tb/M_AXIS_TVALID
add wave -noupdate -expand -group {DUT Interface} -radix hexadecimal /cam_capture_maxis_tb/M_AXIS_VIDEO_TDATA
add wave -noupdate -expand -group {DUT Interface} /cam_capture_maxis_tb/M_AXIS_VIDEO_TUSER
add wave -noupdate -expand -group {DUT Interface} /cam_capture_maxis_tb/M_AXIS_VIDEO_TLAST
add wave -noupdate -expand -group {DUT Internal} /cam_capture_maxis_tb/DUT/STATE
add wave -noupdate -expand -group {DUT Internal} /cam_capture_maxis_tb/DUT/vsync_posedge
add wave -noupdate -expand -group {DUT Internal} /cam_capture_maxis_tb/DUT/pixel_half
add wave -noupdate -expand -group {DUT Internal} /cam_capture_maxis_tb/DUT/pixel_count
add wave -noupdate -expand -group {DUT Internal} /cam_capture_maxis_tb/DUT/row_count
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {5520 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {4996 ps} {6004 ps}
