`include "baudrates.v"

module uart_tx(
         input wire clk,        
         input wire rstn,       
         input wire start,      
         input wire [7:0] data, 
         output reg tx,         
         output wire ready      
);

parameter [15:0] BAUDRATE = `B9600;

reg	[15:0]	tx_period_cnt;
reg	[3:0]	tx_bit_cnt;	// 0: IDLE, 1: Start, 2~9: bit0~7, A:Stop
reg		tx_bit_tick;


always @(posedge clk)
begin
    if(rstn == 1'b0)
	tx_bit_cnt <= 4'b0;
    else if((tx_bit_cnt == 4'b0) && (start == 1'b1))
	tx_bit_cnt <= 4'd1;
    else if((tx_bit_cnt == 4'hA) && tx_bit_tick)
	tx_bit_cnt <= 4'd0;
    else if((tx_bit_cnt != 4'b0) && tx_bit_tick)
	tx_bit_cnt <= tx_bit_cnt + 4'd1;
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	tx_period_cnt <= 16'b0;
    else if(tx_bit_cnt == 4'b0)
	tx_period_cnt <= 16'b0;
    else if(tx_period_cnt == 16'b0)
	tx_period_cnt <= BAUDRATE;
    else
	tx_period_cnt <= tx_period_cnt - 16'd1;
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	tx_bit_tick <= 1'b0;
    else if(tx_period_cnt == 16'd1)
	tx_bit_tick <= 1'b1;
    else
	tx_bit_tick <= 1'b0;
end

assign ready = (tx_bit_cnt == 4'h0);

always @(posedge clk)
begin
    if(rstn == 1'b0)
	tx <= 1'b0;
    else case(tx_bit_cnt)
	4'd1: // start
	    tx <= 1'b0;
	4'd2:
	    tx <= data[0];
	4'd3:
	    tx <= data[1];
	4'd4:
	    tx <= data[2];
	4'd5:
	    tx <= data[3];
	4'd6:
	    tx <= data[4];
	4'd7:
	    tx <= data[5];
	4'd8:
	    tx <= data[6];
	4'd9:
	    tx <= data[7];
	default: // stop & idle
	    tx <= 1'b1;
    endcase
end

endmodule
