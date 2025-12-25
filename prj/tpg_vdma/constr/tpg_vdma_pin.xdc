
##Pmod Header JB (Zybo Z7-20 only)
#set_property -dict { PACKAGE_PIN V8 IOSTANDARD LVCMOS33} [get_ports { SCL_0 }]
#set_property -dict { PACKAGE_PIN W8 IOSTANDARD LVCMOS33} [get_ports { SDA_0 }]
#set_property -dict { PACKAGE_PIN Y7 IOSTANDARD LVCMOS33} [get_ports CAM_PCLK]
#set_property -dict { PACKAGE_PIN Y6 IOSTANDARD LVCMOS33} [get_ports CAM_VSYNC]
#set_property -dict { PACKAGE_PIN V6 IOSTANDARD LVCMOS33} [get_ports CAM_HREF]
#set_property -dict { PACKAGE_PIN W6 IOSTANDARD LVCMOS33} [get_ports CAM_XCLK]
#
###Pmod Header JC
#set_property -dict {PACKAGE_PIN V15 IOSTANDARD LVCMOS33} [get_ports {CAM_DATA[0]}]
#set_property -dict {PACKAGE_PIN W15 IOSTANDARD LVCMOS33} [get_ports {CAM_DATA[1]}]
#set_property -dict {PACKAGE_PIN T11 IOSTANDARD LVCMOS33} [get_ports {CAM_DATA[2]}]
#set_property -dict {PACKAGE_PIN T10 IOSTANDARD LVCMOS33} [get_ports {CAM_DATA[3]}]
#set_property -dict {PACKAGE_PIN W14 IOSTANDARD LVCMOS33} [get_ports {CAM_DATA[4]}]
#set_property -dict {PACKAGE_PIN Y14 IOSTANDARD LVCMOS33} [get_ports {CAM_DATA[5]}]
#set_property -dict {PACKAGE_PIN T12 IOSTANDARD LVCMOS33} [get_ports {CAM_DATA[6]}]
#set_property -dict {PACKAGE_PIN U12 IOSTANDARD LVCMOS33} [get_ports {CAM_DATA[7]}]

##HDMI TX
#set_property -dict { PACKAGE_PIN E18   IOSTANDARD LVCMOS33 } [get_ports { hdmi_tx_hpd }]; #IO_L5P_T0_AD9P_35 Sch=hdmi_tx_hpd
#set_property -dict { PACKAGE_PIN G17   IOSTANDARD LVCMOS33 } [get_ports { hdmi_tx_scl }]; #IO_L16P_T2_35 Sch=hdmi_tx_scl
#set_property -dict { PACKAGE_PIN G18   IOSTANDARD LVCMOS33 } [get_ports { hdmi_tx_sda }]; #IO_L16N_T2_35 Sch=hdmi_tx_sda
set_property -dict {PACKAGE_PIN H17 IOSTANDARD TMDS_33} [get_ports TMDS_0_clk_n]
set_property -dict {PACKAGE_PIN H16 IOSTANDARD TMDS_33} [get_ports TMDS_0_clk_p]
set_property -dict {PACKAGE_PIN D20 IOSTANDARD TMDS_33} [get_ports {TMDS_0_data_n[0]}]
set_property -dict {PACKAGE_PIN D19 IOSTANDARD TMDS_33} [get_ports {TMDS_0_data_p[0]}]
set_property -dict {PACKAGE_PIN B20 IOSTANDARD TMDS_33} [get_ports {TMDS_0_data_n[1]}]
set_property -dict {PACKAGE_PIN C20 IOSTANDARD TMDS_33} [get_ports {TMDS_0_data_p[1]}]
set_property -dict {PACKAGE_PIN A20 IOSTANDARD TMDS_33} [get_ports {TMDS_0_data_n[2]}]
set_property -dict {PACKAGE_PIN B19 IOSTANDARD TMDS_33} [get_ports {TMDS_0_data_p[2]}]