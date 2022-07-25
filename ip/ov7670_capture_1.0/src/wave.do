onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /cam_capture_maxis_tb/DUT/i_sysclk
add wave -noupdate /cam_capture_maxis_tb/DUT/i_resetn
add wave -noupdate -divider {CAM INTERFACE}
add wave -noupdate /cam_capture_maxis_tb/DUT/i_pclk
add wave -noupdate /cam_capture_maxis_tb/DUT/i_vsync
add wave -noupdate /cam_capture_maxis_tb/DUT/i_href
add wave -noupdate /cam_capture_maxis_tb/DUT/i_data
add wave -noupdate -divider {AXI STREAM INTERFACE}
add wave -noupdate /cam_capture_maxis_tb/DUT/M_AXIS_VIDEO_TDATA
add wave -noupdate /cam_capture_maxis_tb/DUT/M_AXIS_TVALID
add wave -noupdate /cam_capture_maxis_tb/DUT/M_AXIS_VIDEO_TREADY
add wave -noupdate /cam_capture_maxis_tb/DUT/M_AXIS_VIDEO_TUSER
add wave -noupdate /cam_capture_maxis_tb/DUT/M_AXIS_VIDEO_TLAST
add wave -noupdate -divider FSM
add wave -noupdate /cam_capture_maxis_tb/DUT/pclk_posedge
add wave -noupdate /cam_capture_maxis_tb/DUT/vsync_posedge
add wave -noupdate /cam_capture_maxis_tb/DUT/STATE
add wave -noupdate /cam_capture_maxis_tb/DUT/pixel_count
add wave -noupdate /cam_capture_maxis_tb/DUT/row_count
add wave -noupdate /cam_capture_maxis_tb/DUT/pixel_half
add wave -noupdate -divider {SYNCED SIGNALS}
add wave -noupdate -group pclk /cam_capture_maxis_tb/DUT/pclk_q1
add wave -noupdate -group pclk /cam_capture_maxis_tb/DUT/pclk_q2
add wave -noupdate -group pclk /cam_capture_maxis_tb/DUT/pclk_q3
add wave -noupdate -group vsync /cam_capture_maxis_tb/DUT/vsync_q1
add wave -noupdate -group vsync /cam_capture_maxis_tb/DUT/vsync_q2
add wave -noupdate -group vsync /cam_capture_maxis_tb/DUT/vsync_q3
add wave -noupdate -group href /cam_capture_maxis_tb/DUT/href_q1
add wave -noupdate -group href /cam_capture_maxis_tb/DUT/href_q2
add wave -noupdate -group data /cam_capture_maxis_tb/DUT/data_q1
add wave -noupdate -group data /cam_capture_maxis_tb/DUT/data_q2
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {19666 ps} 0}
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
WaveRestoreZoom {18412 ps} {20492 ps}
