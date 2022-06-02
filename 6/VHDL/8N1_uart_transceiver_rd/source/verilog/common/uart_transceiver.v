`include "baudrates.v"


module uart_tranceiver(
	input wire clk,
	input wire resetn,


	/////////////TX ports///////////////
	input wire [7:0] 	i_tx_data,
	input wire 			i_tx_data_valid,
	output wire 		o_tx_serial,
	output wire 		o_tx_ready,


	/////////////RX ports///////////////
	input wire 			i_rx_serial,
	output wire [7:0] 	o_rx_data,
	output wire 		o_rx_data_valid
);

parameter BAUDRATE = `B9600;
parameter TX_FIFO_MODE = 1; //default TX FIFO ON
parameter RX_FIFO_MODE = 0; //default RX FIFO OFF
parameter FIFO_DEPTH = 512; //Same value as the DPRAM IP's Address depth


generate if(TX_FIFO_MODE==1'b1)
//TX FIFO_MODE
begin

wire tx_fifo_empty;
wire tx_fifo_full;
wire tx_fifo_rd;
wire tx_fifo_we;
wire [7:0] tx_fifo_dout;
wire tx_ready;

dp_ram_fifo #(.FIFO_DEPTH(FIFO_DEPTH)
)dp_ram_fifo_tx(
	.resetn(		resetn		),
	.i_wr_clk(		clk			),
	.i_rd_clk(		clk			),
	.i_wr_clk_en(	1'b1		),
	.i_rd_clk_en(	1'b1		),
	.i_wr_en(		tx_fifo_we		),
	.i_rd_en(		tx_fifo_rd		),
	.i_wr_data(		i_tx_data	),
	.o_rd_data(		tx_fifo_dout	),
	.fifo_full(		tx_fifo_full	),
	.fifo_empty(	tx_fifo_empty	)
);

assign tx_fifo_rd = !tx_fifo_empty && tx_ready;
assign tx_fifo_we = !tx_fifo_full && i_tx_data_valid;
assign o_tx_ready = !tx_fifo_full;

uart_tx #(.BAUDRATE(BAUDRATE)  //-- Default baudrate
) uart_tx_u0(
	.clk(			clk				), //-- System clock 12Mhz
    .rstn(			resetn			), //-- Reset  (Active low)
    .start(			tx_fifo_rd		), //-- Set to 1 for starting the transmission
    .data(			tx_fifo_dout	), //-- Byte to transmit
    .tx(			o_tx_serial		), //-- Serial o_rx_data output
    .ready(			tx_ready		)  //-- Transmitter ready (1) / busy (0)
);

end
//TX NON-FIFO MODE
else begin

reg [7:0]tx_data_r;

always@(posedge clk)
	if(i_tx_data_valid==1 && o_tx_ready==1)
		tx_data_r = i_tx_data;

uart_tx #(.BAUDRATE(BAUDRATE)  //-- Default baudrate
) uart_tx_u0(
	.clk(			clk				), //-- System clock 12Mhz
    .rstn(			resetn			), //-- Reset  (Active low)
    .start(			i_tx_data_valid	), //-- Set to 1 for starting the transmission
    .data(			tx_data_r		), //-- Byte to transmit
    .tx(			o_tx_serial		), //-- Serial o_rx_data output
    .ready(			o_tx_ready		)  //-- Transmitter ready (1) / busy (0)
);
end
endgenerate


//RX FIFO MODE
generate if(RX_FIFO_MODE == 1)
begin

wire rx_fifo_empty;
wire rx_fifo_full;
wire rx_fifo_rd;
wire rx_fifo_we;
wire [7:0] rx_fifo_din;
wire  rx_fifo_din_valid;
reg  rx_data_valid;

uart_rx #(.BAUDRATE(BAUDRATE)   //-- Default baudrate
) uart_rx_u0(	
	.clk(			clk				), //-- System clock 12Mhz
	.rstn(			resetn			), //-- Reset (Active low)
    .rx(			i_rx_serial		), //-- Serial o_rx_data input
    .rcv(			rx_fifo_din_valid	), //--data is available (1)
    .data(			rx_fifo_din		)  //-- data received
);

dp_ram_fifo dp_ram_fifo_rx(
	.resetn(		resetn		),
	.i_wr_clk(		clk			),
	.i_rd_clk(		clk			),
	.i_wr_clk_en(	1'b1		),
	.i_rd_clk_en(	1'b1		),
	.i_wr_en(		rx_fifo_we	),
	.i_rd_en(		rx_fifo_rd	),
	.i_wr_data(		rx_fifo_din	),
	.o_rd_data(		o_rx_data	),
	.fifo_full(		rx_fifo_full	),
	.fifo_empty(	rx_fifo_empty	)
);

assign rx_fifo_we = !rx_fifo_full && rx_fifo_din_valid;
assign rx_fifo_rd = !rx_fifo_empty;

always@(posedge clk)
	if(resetn==0)
		rx_data_valid = 0;
	else if(rx_fifo_rd)
		rx_data_valid = 1;
	else
		rx_data_valid = 0;
		
assign o_rx_data_valid = rx_data_valid;		
		
end

//RX NON-FIFO MODE
else begin
	
uart_rx #(.BAUDRATE(BAUDRATE)   //-- Default baudrate
) uart_rx_u0(	
	.clk(			clk				), //-- System clock 12Mhz
	.rstn(			resetn			), //-- Reset (Active low)
    .rx(			i_rx_serial		), //-- Serial o_rx_data input
    .rcv(			o_rx_data_valid	), //--data is available (1)
    .data(			o_rx_data		)  //-- data received
);
end
endgenerate




endmodule