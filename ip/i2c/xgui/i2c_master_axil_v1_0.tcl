# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "CMD_FIFO" -parent ${Page_0}
  ipgui::add_param $IPINST -name "CMD_FIFO_ADDR_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "DEFAULT_PRESCALE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "FIXED_PRESCALE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "READ_FIFO" -parent ${Page_0}
  ipgui::add_param $IPINST -name "READ_FIFO_ADDR_WIDTH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "WRITE_FIFO" -parent ${Page_0}
  ipgui::add_param $IPINST -name "WRITE_FIFO_ADDR_WIDTH" -parent ${Page_0}


}

proc update_PARAM_VALUE.CMD_FIFO { PARAM_VALUE.CMD_FIFO } {
	# Procedure called to update CMD_FIFO when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CMD_FIFO { PARAM_VALUE.CMD_FIFO } {
	# Procedure called to validate CMD_FIFO
	return true
}

proc update_PARAM_VALUE.CMD_FIFO_ADDR_WIDTH { PARAM_VALUE.CMD_FIFO_ADDR_WIDTH } {
	# Procedure called to update CMD_FIFO_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CMD_FIFO_ADDR_WIDTH { PARAM_VALUE.CMD_FIFO_ADDR_WIDTH } {
	# Procedure called to validate CMD_FIFO_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.DEFAULT_PRESCALE { PARAM_VALUE.DEFAULT_PRESCALE } {
	# Procedure called to update DEFAULT_PRESCALE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DEFAULT_PRESCALE { PARAM_VALUE.DEFAULT_PRESCALE } {
	# Procedure called to validate DEFAULT_PRESCALE
	return true
}

proc update_PARAM_VALUE.FIXED_PRESCALE { PARAM_VALUE.FIXED_PRESCALE } {
	# Procedure called to update FIXED_PRESCALE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.FIXED_PRESCALE { PARAM_VALUE.FIXED_PRESCALE } {
	# Procedure called to validate FIXED_PRESCALE
	return true
}

proc update_PARAM_VALUE.READ_FIFO { PARAM_VALUE.READ_FIFO } {
	# Procedure called to update READ_FIFO when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.READ_FIFO { PARAM_VALUE.READ_FIFO } {
	# Procedure called to validate READ_FIFO
	return true
}

proc update_PARAM_VALUE.READ_FIFO_ADDR_WIDTH { PARAM_VALUE.READ_FIFO_ADDR_WIDTH } {
	# Procedure called to update READ_FIFO_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.READ_FIFO_ADDR_WIDTH { PARAM_VALUE.READ_FIFO_ADDR_WIDTH } {
	# Procedure called to validate READ_FIFO_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.WRITE_FIFO { PARAM_VALUE.WRITE_FIFO } {
	# Procedure called to update WRITE_FIFO when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.WRITE_FIFO { PARAM_VALUE.WRITE_FIFO } {
	# Procedure called to validate WRITE_FIFO
	return true
}

proc update_PARAM_VALUE.WRITE_FIFO_ADDR_WIDTH { PARAM_VALUE.WRITE_FIFO_ADDR_WIDTH } {
	# Procedure called to update WRITE_FIFO_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.WRITE_FIFO_ADDR_WIDTH { PARAM_VALUE.WRITE_FIFO_ADDR_WIDTH } {
	# Procedure called to validate WRITE_FIFO_ADDR_WIDTH
	return true
}


proc update_MODELPARAM_VALUE.DEFAULT_PRESCALE { MODELPARAM_VALUE.DEFAULT_PRESCALE PARAM_VALUE.DEFAULT_PRESCALE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DEFAULT_PRESCALE}] ${MODELPARAM_VALUE.DEFAULT_PRESCALE}
}

proc update_MODELPARAM_VALUE.FIXED_PRESCALE { MODELPARAM_VALUE.FIXED_PRESCALE PARAM_VALUE.FIXED_PRESCALE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.FIXED_PRESCALE}] ${MODELPARAM_VALUE.FIXED_PRESCALE}
}

proc update_MODELPARAM_VALUE.CMD_FIFO { MODELPARAM_VALUE.CMD_FIFO PARAM_VALUE.CMD_FIFO } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.CMD_FIFO}] ${MODELPARAM_VALUE.CMD_FIFO}
}

proc update_MODELPARAM_VALUE.CMD_FIFO_ADDR_WIDTH { MODELPARAM_VALUE.CMD_FIFO_ADDR_WIDTH PARAM_VALUE.CMD_FIFO_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.CMD_FIFO_ADDR_WIDTH}] ${MODELPARAM_VALUE.CMD_FIFO_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.WRITE_FIFO { MODELPARAM_VALUE.WRITE_FIFO PARAM_VALUE.WRITE_FIFO } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.WRITE_FIFO}] ${MODELPARAM_VALUE.WRITE_FIFO}
}

proc update_MODELPARAM_VALUE.WRITE_FIFO_ADDR_WIDTH { MODELPARAM_VALUE.WRITE_FIFO_ADDR_WIDTH PARAM_VALUE.WRITE_FIFO_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.WRITE_FIFO_ADDR_WIDTH}] ${MODELPARAM_VALUE.WRITE_FIFO_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.READ_FIFO { MODELPARAM_VALUE.READ_FIFO PARAM_VALUE.READ_FIFO } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.READ_FIFO}] ${MODELPARAM_VALUE.READ_FIFO}
}

proc update_MODELPARAM_VALUE.READ_FIFO_ADDR_WIDTH { MODELPARAM_VALUE.READ_FIFO_ADDR_WIDTH PARAM_VALUE.READ_FIFO_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.READ_FIFO_ADDR_WIDTH}] ${MODELPARAM_VALUE.READ_FIFO_ADDR_WIDTH}
}

