module uart8n1_rx2(
        input wire clk,
        input wire en ,
        input wire rxIn ,
        output wire xDv,
        output wire isIdle,
        output wire [7:0] rxByte
    );

//subtype st_logicBit is std_ulogic;
//subtype st_bit is unsigned(0 downto 0);
//subtype st_byte is unsigned(7 downto 0);
//subtype state_t is unsigned(2 downto 0);
reg [7:0] rxByteSignal;
localparam [2:0] StateIdle = 0;
localparam [2:0] StateStartRx = 1;
localparam [2:0] StateRxing = 2;
localparam [2:0] StateRxDone = 3;
localparam [2:0] StateRxCleanup = 4;

reg [2:0] state = 0; 
reg [7:0] bufTx = 0;
reg [31:0] bitIndex = 0;
reg [31:0] posEdgeCounter = 0;
reg [31:0] g_CLKS_PER_BIT = 1250;
assign rxByte = rxByteSignal;
always @(posedge clk) begin
	if (en == 0) begin
                isIdle <= 1;
	end else begin
            if (state == StateIdle) begin
                isIdle <= 1;
                rxDv <= 0;
                posEdgeCounter = 0;
                bitIndex = 0;

                if (rxIn == 0) begin
		    isIdle <= 0;
                    state = StateStartRx;

                end else begin
                    state = StateIdle;
                end;

            end else if (state == StateStartRx) begin
                isIdle <= 0;
                if (posEdgeCounter == (g_CLKS_PER_BIT-1)/2) begin
                    if (rxIn == 0) begin
                        posEdgeCounter = 0;
                        state = StateRxing;
                    end else begin
                        state = StateIdle;
                    end;

                end else begin
                    posEdgeCounter = posEdgeCounter + 1;
                    state = StateStartRx;
                end;

            end else if (state == StateRxing) begin
                isIdle <= 0;
                if (posEdgeCounter < g_CLKS_PER_BIT-1) begin
                    posEdgeCounter = posEdgeCounter + 1;
                    state = StateRxing;

                end else begin
                    posEdgeCounter = 0;

                    if (rxIn == 1) begin
                        rxByteSignal[bitIndex] <= 1;
                    end else begin
                        rxByteSignal[bitIndex] <= 0;
                    end;

                    if (bitIndex < 7) begin
                        bitIndex = bitIndex + 1;
                        state = StateRxing;

                    end else begin
                        bitIndex = 0;
                        state = StateRxDone;
                    end;
                end;

            end else if (state == StateRxDone) begin
                isIdle <= 0;
                if (posEdgeCounter < (g_CLKS_PER_BIT - 1)) begin
                    posEdgeCounter = posEdgeCounter + 1;
                    state = StateRxDone;

                end else begin
                    rxDv <= 1;
                    posEdgeCounter = 0;
                    state = StateRxCleanup;
                end;

            end else if (state == StateRxCleanup) begin
                isIdle <= 0;
                state = StateIdle;
                rxDv <= 0;

            end;

    end
end;
endmodule
