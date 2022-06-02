module dp_ram_fifo(
input wire resetn,
input wire i_wr_clk,
input wire i_rd_clk,
input wire i_wr_clk_en,
input wire i_rd_clk_en,
input wire i_wr_en,
input wire i_rd_en,
input wire [7:0] i_wr_data,
output wire [7:0] o_rd_data,
output reg fifo_full,
output reg fifo_empty
);
parameter FIFO_DEPTH = 512;


reg	[8:0]	fifo_waddr;
reg	[8:0]	fifo_raddr;
reg [8:0]	fifo_counter;


//fifo_waddr
always @(posedge i_wr_clk or negedge resetn)
begin
    if(resetn == 1'b0)
	fifo_waddr <= 9'b0;
    else if(i_wr_en)
	fifo_waddr <= fifo_waddr + 9'd1;
end//fifo_waddr



//fifo_raddr{{
always @(posedge i_rd_clk or negedge resetn)
begin
    if(resetn == 1'b0)
	fifo_raddr <= 9'b0;
    else if(i_rd_en)
	fifo_raddr <= fifo_raddr + 9'd1;
end//fifo_raddr


//Compare Logic{{
always @(fifo_counter)
begin
   fifo_empty = (fifo_counter==0);
   fifo_full = (fifo_counter== FIFO_DEPTH - 1);
end

always @(posedge i_rd_clk or negedge resetn)
begin
   if( resetn == 0 )
       fifo_counter <= 0;

   else if( (!fifo_full && i_wr_en) && ( !fifo_empty && i_rd_en ) )
       fifo_counter <= fifo_counter;

   else if( !fifo_full && i_wr_en )
       fifo_counter <= fifo_counter + 1;

   else if( !fifo_empty && i_rd_en )
       fifo_counter <= fifo_counter - 1;
   else
      fifo_counter <= fifo_counter;
end//}}Compare Logic

	
dpram dpram_u0(
		.Reset(~resetn),
		.WE(i_wr_en),
		.WrClock(i_wr_clk ),
        .RdClock(i_rd_clk ),
        //.WrClockEn(i_wr_clk_en ),
        .RdClockEn(i_rd_en ),
        //.RdClockEn(i_rd_clk_en ),
        .WrClockEn(i_wr_en ),
        .Data(i_wr_data ),
        .WrAddress(fifo_waddr[8:0] ),
        .RdAddress(fifo_raddr[8:0] ),
        .Q(o_rd_data ));	

endmodule