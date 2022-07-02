`timescale 1ns / 1ps

module servomain(input clk, input clr, input [9:0] pos, output reg servo);
  reg [19:0] counter;
    
 always @(*) 
  begin
    if (pos > counter[19:9]) // counter[19:9] increments each time counter[8:0] equals 511
    begin                           // counter[19:9] max value is 4095
      servo = 1'b1;
    end 
    else 
    begin
      servo = 1'b0;
    end
  end
    
  always @(posedge clk, posedge clr) // counter increments, resets back to 0 upon overflow at max value of ~ 1 million
  begin 
    if (clr)
    begin
      counter <= 1'b0;
    end 
    else 
    begin
      counter <= counter + 1'b1;
    end
  end
  
endmodule

	
