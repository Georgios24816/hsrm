`include "baudrates.v"

module uart_rx(
input       		clk,
output reg [7:0]	data,
output reg      	rcv,
input				rx,
input           	rstn
);

parameter [15:0] BAUDRATE = `B9600; 

reg	[15:0]	rx_period_cnt;
reg	[3:0]	rx_bit_cnt;	// 0: IDLE, 1: Start, 2~9: bit0~7, A:Stop
reg		rx_bit_tick;
reg		rx_sample_tick;

reg	[1:0]	rxd_lat;
reg	[7:0]	rxd_shift;

reg		rx_valid_tg;
reg	[1:0]	rx_valid_tg_clk;

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rxd_lat <= 2'b0;
    else if(rx_bit_cnt==4'hA) //During Stop State, force register to bit 1 until IDLE 
	rxd_lat <= {rxd_lat[0], 1'b1};
	else
	rxd_lat <= {rxd_lat[0], rx};
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rxd_shift <= 8'b0;
    else if(rx_sample_tick)
	rxd_shift <= {rxd_lat[0], rxd_shift[7:1]};
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rx_valid_tg <= 1'b0;
    else if(rx_sample_tick & (rx_bit_cnt == 4'hA))
	rx_valid_tg <= !rx_valid_tg;
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	data <= 8'b0;
    else if(rx_sample_tick & (rx_bit_cnt == 4'hA))
	data <= rxd_shift;
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rx_bit_cnt <= 4'b0;
    else if((rx_bit_cnt == 4'b0) && (rxd_lat == 2'b10))
	rx_bit_cnt <= 4'd1;
    else if((rx_bit_cnt == 4'hA) && rx_bit_tick)
	rx_bit_cnt <= 4'd0;
    else if((rx_bit_cnt != 4'b0) && rx_bit_tick)
	rx_bit_cnt <= rx_bit_cnt + 4'd1;
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rx_period_cnt <= 16'b0;
    else if(rx_bit_cnt == 4'b0)
	rx_period_cnt <= 16'b0;
    else if(rx_period_cnt == 16'b0)
	rx_period_cnt <= BAUDRATE;
    else
	rx_period_cnt <= rx_period_cnt - 16'd1;
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rx_bit_tick <= 1'b0;
    else if(rx_period_cnt == 16'd1)
	rx_bit_tick <= 1'b1;
    else
	rx_bit_tick <= 1'b0;
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rx_sample_tick <= 1'b0;
    else if(rx_period_cnt == {1'b0, BAUDRATE[15:1]})
	rx_sample_tick <= 1'b1;
    else
	rx_sample_tick <= 1'b0;
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rx_valid_tg_clk <= 2'b0;
    else 
	rx_valid_tg_clk <= {rx_valid_tg_clk[0], rx_valid_tg};
end

always @(posedge clk)
begin
    if(rstn == 1'b0)
	rcv <= 1'b0;
    else 
	rcv <= (rx_valid_tg_clk[0] != rx_valid_tg_clk[1]);
end



endmodule
