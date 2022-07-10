module cam_capture_maxis_tb();

// TEST PARAMETERS
    parameter T_PCLK = 40;  // simulated pixel clock period 
    parameter X_RES  = 64;  // number of pixels per row 
    parameter Y_RES  = 3;   // number of rows per frame 

    parameter TEST_RUNS = 3; // number of test frames to generate 
 
// LOCAL LOGIC
    logic        i_resetn;
    logic        i_enable;
    logic        i_pclk;
    logic        i_vsync;
    logic        i_href;
    logic [7:0]  i_data;

    logic [15:0] M_AXIS_VIDEO_TDATA;
    logic        M_AXIS_TVALID;
    logic        M_AXIS_VIDEO_TUSER;
    logic        M_AXIS_VIDEO_TLAST;

// DUT INSTANTIATION
    cam_capture_maxis 
    #(.X_RES(X_RES),
      .Y_RES(Y_RES))
    DUT (
    .i_resetn (i_resetn),
    .i_enable (i_enable),
    .i_pclk   (i_pclk),
    .i_vsync  (i_vsync),
    .i_href   (i_href),
    .i_data   (i_data),
    .M_AXIS_VIDEO_TDATA  (M_AXIS_VIDEO_TDATA),
    .M_AXIS_TVALID       (M_AXIS_TVALID),
    .M_AXIS_VIDEO_TREADY (),
    .M_AXIS_VIDEO_TUSER  (M_AXIS_VIDEO_TUSER),
    .M_AXIS_VIDEO_TLAST  (M_AXIS_VIDEO_TLAST)
    );

// CLOCK GEN
    initial i_pclk = 0;
    always#(T_PCLK/2) i_pclk = !i_pclk;

// SIM STRUCT
    typedef struct {
        logic [15:0] queue [$];
        int pixelCount;
    } cam;
    cam camInst;
    cam capInst;
    
// SIM TASKS

// Task to generate a vsync pulse.
    localparam LINE_CNTS = X_RES*2;
    task sendVsync;
    begin
        @(negedge i_pclk) i_vsync <= 1;
        for(int i=0; i<LINE_CNTS; i++) begin 
            @(negedge i_pclk);
        end 
        @(negedge i_pclk) i_vsync <= 0;
    end 
    endtask 

// Task to send a pixel, comprised of two bytes.
// Has parameters to generate a SoL condition or EoL condition.
    task sendPixel;
    input bit SoL;
    input bit EoL;
    logic [15:0] randomData;
    begin 
        @(negedge i_pclk) 
            if(SoL) i_href <= 1; // on SoL, href goes high with the first byte
            randomData = $random;
            camInst.queue.push_back(randomData);
            i_data <= randomData[7:0];

        @(negedge i_pclk) 
            i_data <= randomData[15:8];
        
        // on EoL, ref goes low on the negedge pclk after the last byte
        if(EoL) begin 
            @(negedge i_pclk) i_href <= 0;
        end 
    end 
    endtask : sendPixel 

// Task to send a row of pixels, length of which determined by parameter X_RES.
    task sendRow;
    begin 
        for(int i=0; i<X_RES; i++) begin 
            if(i==0)            sendPixel(1,0);
            else if(i==X_RES-1) sendPixel(0,1);
            else                sendPixel(0,0);
        end 
        @(negedge i_pclk);
    end 
    endtask : sendRow 

// Process to continuously register data out from the DUT.
    always@(posedge i_pclk) begin 
        if(!i_resetn) begin 
            capInst.queue.delete();
        end 
        else if(M_AXIS_TVALID) begin 
            capInst.queue.push_back(M_AXIS_VIDEO_TDATA);
        end 
    end 

// Task to check captured data vs sent
    task checkData;
    begin 
        foreach (camInst.queue[i]) begin 
            if(camInst.queue[i] == capInst.queue[i]) begin 
                $display("PASS! Index [%d] | Camera sent: %h  |  DUT Captured: %h", i, camInst.queue[i], capInst.queue[i]);
            end 
            else begin 
                $display("FAIL! Mismatch at [%d] | Camera sent: %h  |  DUT Captured: %h", i, camInst.queue[i], capInst.queue[i]);
                $stop();
            end
        end 
        camInst.queue.delete();
        capInst.queue.delete();
    end 
    endtask 

// Task to send a video frame and check it.
    task sendFrame;
    input int num;
    begin 
        camInst.queue.delete();
        capInst.queue.delete();
        $display("\n\nSending Frame %d\n\n", num);
        sendVsync();
        for(int i=0; i<Y_RES; i++) begin 
            $display("\nLine %0d of Frame %0d starting...\n", i, num);
            sendRow();
            checkData();
        end 
    end 
    endtask 



// Process to monitor timing signals
    initial begin 
        i_resetn = 0;
        i_enable = 0;
        i_vsync  = 0;
        i_href   = 0;
        i_data   = 0;
        $monitor("INFO: T=%t | vsync=%b | href=%b | tuser=%b | tlast %b", $time, i_vsync, i_href, M_AXIS_VIDEO_TUSER, M_AXIS_VIDEO_TLAST);
        #100;
        i_resetn = 1;
        i_enable = 1;
        repeat(3) @(negedge i_pclk);  

        //
        for(int i=0; i<TEST_RUNS; i++) begin 
            sendFrame(i);
        end 
        //
        #100;
        $stop;
    end 

    

endmodule
