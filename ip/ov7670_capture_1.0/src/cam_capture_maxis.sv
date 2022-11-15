

module cam_capture_maxis
    #(parameter X_RES = 640,
      parameter Y_RES = 480 )
    (
    input  logic        i_resetn, // async active-low reset
    input  logic        i_enable, 
 
    // OV7670 interface  
    input  logic        i_pclk,   // pixel clock 
    input  logic        i_vsync,  // active-high vsync
    input  logic        i_href,   // active-high href
    input  logic [7:0]  i_data,   // 8-bit input data

    // AXI-Stream Master Interface
    output logic [15:0] M_AXIS_VIDEO_TDATA,
    output logic        M_AXIS_TVALID,
    input  logic        M_AXIS_VIDEO_TREADY, // unused
    output logic        M_AXIS_VIDEO_TUSER,  // SoF
    output logic        M_AXIS_VIDEO_TLAST   // EoL
    );

// LOCAL LOGIC
    typedef enum {
        ST_IDLE, ST_ACTIVE
    } fsm_state_encoding;

    fsm_state_encoding STATE;

    logic [$clog2(X_RES)-1 :0] pixel_count;
    logic [$clog2(Y_RES)-1 :0] row_count;
    logic                      pixel_half;

    logic                      vsync_q;
    logic                      vsync_posedge;


// VSYNC POSEDGE DETECTOR
    always_ff@(posedge i_pclk or negedge i_resetn) begin 
        if(!i_resetn) vsync_q <= 0;
        else vsync_q <= i_vsync; 
    end 
    assign vsync_posedge = (!vsync_q) && i_vsync;

// FSM
    always_ff@(posedge i_pclk or negedge i_resetn) begin 
        if(!i_resetn || !i_enable) begin 
            M_AXIS_VIDEO_TDATA  <= 0;
            M_AXIS_TVALID       <= 0;
            M_AXIS_VIDEO_TUSER  <= 0;
            M_AXIS_VIDEO_TLAST  <= 0;

            STATE               <= ST_IDLE;
            pixel_count         <= 0;
            row_count           <= 0;
            pixel_half          <= 0;
        end 
        else begin 
            M_AXIS_VIDEO_TUSER <= (pixel_count == 0) && (row_count == 0);
            M_AXIS_VIDEO_TLAST <= (pixel_count == X_RES-1);

            case(STATE) 
                ST_IDLE: begin 
                    M_AXIS_TVALID <= 0;
                    pixel_count   <= 0;
                    row_count     <= 0;
                    pixel_half    <= 0;
                    if(vsync_posedge) STATE <= ST_ACTIVE;
                end 

                ST_ACTIVE: begin 
                    if(!i_href) begin 
                        M_AXIS_TVALID <= 0;
                    end 
                    else begin
                        pixel_half <= !pixel_half;

                        if(!pixel_half) begin
                            M_AXIS_TVALID <= 0;
                            M_AXIS_VIDEO_TDATA[7:0] <= i_data;
                        end 
                        else begin 
                            M_AXIS_TVALID <= 1;
                            M_AXIS_VIDEO_TDATA[15:8] <= i_data;

                            if(pixel_count == X_RES-1) begin 
                                pixel_count <= 0;
                                if(row_count == Y_RES-1) STATE <= ST_IDLE; 
                                else row_count <= row_count+1;  
                            end 
                            else begin 
                                pixel_count <= pixel_count+1;
                            end 
                        end 
                    end 
                end 

            endcase
        end 
    end 

endmodule