`include "baudrates.v"

module uart_ice40up_top(
	input i_rx_serial,
	//input resetn,
	output o_tx_serial,
	output o_tx_serial_duplicate
);

parameter BAUDRATE = `B9600;

wire resetn;
wire clk;
wire [7:0] data;
wire data_valid;

uart_tranceiver #(.BAUDRATE(BAUDRATE),
				.TX_FIFO_MODE(1),
				.RX_FIFO_MODE(0)
) uart_transceiver_u0(
	.clk(clk),
	.resetn(resetn),


	/////////////TX ports///////////////
	.i_tx_data(data),
	.i_tx_data_valid(data_valid),
	.o_tx_serial(o_tx_serial),
	.o_tx_ready(),


	/////////////RX ports///////////////
	.i_rx_serial(i_rx_serial),
	.o_rx_data(data),
	.o_rx_data_valid(data_valid)
);

assign o_tx_serial_duplicate = o_tx_serial; //for debugging

HSOSC # (
	.CLKHF_DIV("0b10")) u_hfosc ( //System clock 12Mhz
	.CLKHFEN   (1'b1 ),
	.CLKHFPU   (1'b1 ),
	.CLKHF     (clk)
			);

reset reset1(
	.clk(clk),
	.reset(resetn)
	);

endmodule