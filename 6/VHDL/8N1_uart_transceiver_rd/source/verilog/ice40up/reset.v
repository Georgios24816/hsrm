`timescale 1ms / 1us

module reset(
input wire clk, 
output wire reset
);

wire	[7:0]	cnt;
wire	[7:0]	cnt_in;
wire		cnt_en;
wire	        r_resetn;

FD1P3XZ u_cnt_reg [7:0] (
    .D(cnt_in),
    .SP(cnt_en),
    .CK(clk),
    .SR(1'b0),
    .Q(cnt));
    defparam u_cnt_reg[7].REGSET = "RESET";
    defparam u_cnt_reg[6].REGSET = "RESET";
    defparam u_cnt_reg[5].REGSET = "RESET";
    defparam u_cnt_reg[4].REGSET = "RESET";
    defparam u_cnt_reg[3].REGSET = "RESET";
    defparam u_cnt_reg[2].REGSET = "RESET";
    defparam u_cnt_reg[1].REGSET = "RESET";
    defparam u_cnt_reg[0].REGSET = "RESET";
    defparam u_cnt_reg[7].SRMODE = "CE_OVER_LSR";
    defparam u_cnt_reg[6].SRMODE = "CE_OVER_LSR";
    defparam u_cnt_reg[5].SRMODE = "CE_OVER_LSR";
    defparam u_cnt_reg[4].SRMODE = "CE_OVER_LSR";
    defparam u_cnt_reg[3].SRMODE = "CE_OVER_LSR";
    defparam u_cnt_reg[2].SRMODE = "CE_OVER_LSR";
    defparam u_cnt_reg[1].SRMODE = "CE_OVER_LSR";
    defparam u_cnt_reg[0].SRMODE = "CE_OVER_LSR";

FD1P3XZ u_r_resetn0 (
    .D(1'b1 ),
    .SP(!cnt_en),
    .CK(clk),
    .SR(1'b0),
    .Q(r_resetn));
    defparam u_r_resetn0.REGSET = "RESET";
    defparam u_r_resetn0.SRMODE = "CE_OVER_LSR";

assign reset = r_resetn;
assign cnt_in = cnt + 8'd1;
assign cnt_en = (cnt != 8'hff);

endmodule
