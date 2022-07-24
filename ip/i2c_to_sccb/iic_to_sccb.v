/* iic_to_sccb.sv
 * 
 * This module provides a bridge from the output of a Xilinx AXI IIC IP
 * to an Omnivision SCCB bus.
 *
 * SCCB SIO_C is:
 *     - single-directional 
 *     - active-high
 *     - driven by the master
 *
 * SCCB SIO_D is:
 *     - bi-directional
 *     - floating when bus is idle
*/

module iic_to_sccb 
    (
    // SCCB Interface
    output wire SIOC,
    inout  wire SIOD,

    // AXI IIC Interface
    output wire scl_i, // SCL data input
    input  wire scl_o, // SCL data output
    input  wire scl_t, // SCL tristate active-high enable

    output wire sda_i, // SDA data input
    input  wire sda_o, // SDA data output
    input  wire sda_t  // SDA tristate active-high enable 
    );

/*
|  scl_t  |  bus state  |
|    1    |      1      |
|    0    |      0      |
*/
    assign SIOC  = scl_t;
    assign scl_i = scl_t;

    IOBUF iofbuf_inst (
        .O  (sda_i), // buffer output
        .IO (SIOD),  // buffer inout port
        .I  (sda_o), // buffer input
        .T  (sda_t)  // tristate enable 
    );


endmodule 