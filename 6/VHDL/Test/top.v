/* Top level module for keypad + UART demo */
module top (
    // input hardware clock (12 MHz)
    hwclk, 
    // all LEDs
    led1, led4, ledGreen, led3, led2, 
    // UART lines
    ftdi_tx, ftdi_rx
    );

    /* Clock input */
    input hwclk;

    output led4;
    output led1;
    output led2;
    output led3;
    output ledGreen;

    /* FTDI I/O */
    output ftdi_tx;
    input ftdi_rx;

    /* 9600 Hz clock generation (from 12 MHz) */
    reg clk_9600 = 0;
    reg [31:0] cntr_9600 = 32'b0;
    parameter period_9600 = 625;

    /* 1 Hz clock generation (from 12 MHz) */
    reg clk_1 = 0;
    reg [31:0] cntr_1 = 32'b0;
    parameter period_1 = 6000000;

    // Note: could also use "0" or "9" below, but I wanted to
    // be clear about what the actual binary value is.
    parameter ASCII_0 = 8'd48;
    parameter ASCII_9 = 8'd57;

    /* UART registers */
    reg [7:0] uart_txbyte = 0;
    reg uart_send = 0;
    wire uart_txed;
	reg uart_send_en = 0;
    /* LED register */
    reg ledval = 0;

    /////
    
    reg [3 : 0] UartStateCounter = 15;
    reg receivedByte = 0;

    reg rxDvSignal;
    reg isIdleSignal = 1;
    reg [7:0] uart_rxbyte;
    reg rxEnSignal = 0;

    //uart8n1_rx2 tarns(
    //    .clk (hwclk),
    //    .en (rxEnSignal),
    //    .rxIn (ftdi_rx),
    //    .xDv (rxDvSignal),
    //    .isIdle (isIdleSignal),
    //    .rxByte (uart_rxbyte)
    //);
    //

    /* UART transmitter module designed for
       8 bits, no parity, 1 stop bit. 
    */
    uart_tx_8n1 transmitter (
        // 9600 baud rate clock
        .clk (clk_9600),
        // byte to be transmitted
        .txbyte (uart_txbyte),
        // trigger a UART transmit on baud clock
        .senddata (uart_send),
        // input: tx is finished
        .txdone (uart_txed),
        // output UART tx pin
        .tx (ftdi_tx),
	    .en (uart_send_en)
	    //.debugLed(led2)
    );

    wire chachaStart = 1;
    wire [255 : 0] chachaKey = 256'h657870616e642033322d62797465206b657870616e642033322d62797465206b;
    wire [63 : 0] chachaIndex = 0;
    wire [63 : 0] chachaNonce = 64'h6578706164312887;
    reg chachaDone;
    wire [511 : 0] chachaOut;

    chacha20 chacha (
        .clock(hwclk),
        .start(chachaStart),
        .key(chachaKey),
        .index(chachaIndex),
        .nonce(chachaNonce),
        .done(chachaDone),
        .out(chachaOut)
        //.led(ledGreen)
    );

    reg [15 : 0] outputIndex = 0;
    reg [15 : 0] temp1 = 0;
    reg [15 : 0] temp2 = 7;
    
    /* Low speed clock generation */
    always @ (posedge hwclk) begin
        /* generate 9600 Hz clock */
        cntr_9600 <= cntr_9600 + 1;
        if (cntr_9600 == period_9600) begin
            clk_9600 <= ~clk_9600;
            cntr_9600 <= 32'b0;
        end

        /* generate 1 Hz clock */
        cntr_1 <= cntr_1 + 1;
        if (cntr_1 == period_1) begin
            clk_1 <= ~clk_1;
            cntr_1 <= 32'b0;
        end

        if (UartStateCounter == 0) begin
            chachaStart <= 0;
		    if (uart_txed == 1 && chachaDone == 1) begin
                if (chachaOut != 0) begin
				    chachaStart <= 0;
				    UartStateCounter <= 1;
				    led1 <= 1;
                end
			end 

	    end else if (UartStateCounter == 1) begin
			if (uart_txed == 1) begin
				UartStateCounter <= 2;
                uart_txbyte <= chachaOut[7 : 0];
                temp1 <= temp1 + 8;
                //temp2 <= temp2 + 8;
                /*
                if (outputIndex == 0) begin
				    uart_txbyte <= chachaOut[7 : 0];
                end else if (outputIndex == 1) begin
                    uart_txbyte <= chachaOut[15 : 8];
                end else if (outputIndex == 2) begin
                    uart_txbyte <= chachaOut[23 : 16];
                end else if (outputIndex ==3) begin 
                    uart_txbyte <= chachaOut[31:24];
                end else if (outputIndex ==4) begin 
                    uart_txbyte <= chachaOut[39:32];
                end else if (outputIndex ==5) begin 
                    uart_txbyte <= chachaOut[47:40];
                end else if (outputIndex ==6) begin 
                    uart_txbyte <= chachaOut[55:48];
                end else if (outputIndex ==7) begin 
                    uart_txbyte <= chachaOut[63:56];
                end else if (outputIndex ==8) begin 
                    uart_txbyte <= chachaOut[71:64];
                end else if (outputIndex ==9) begin 
                    uart_txbyte <= chachaOut[79:72];
                end else if (outputIndex ==10) begin 
                    uart_txbyte <= chachaOut[87:80];
                end else if (outputIndex ==11) begin 
                    uart_txbyte <= chachaOut[95:88];
                end else if (outputIndex ==12) begin 
                    uart_txbyte <= chachaOut[103:96];
                end else if (outputIndex ==13) begin 
                    uart_txbyte <= chachaOut[111:104];
                end else if (outputIndex ==14) begin 
                    uart_txbyte <= chachaOut[119:112];
                end else if (outputIndex ==15) begin 
                    uart_txbyte <= chachaOut[127:120];
                end else if (outputIndex ==16) begin 
                    uart_txbyte <= chachaOut[135:128];
                end else if (outputIndex ==17) begin 
                    uart_txbyte <= chachaOut[143:136];
                //end else if (outputIndex ==18) begin 
                //    uart_txbyte <= chachaOut[151:144];
                //end else if (outputIndex ==19) begin 
                //    uart_txbyte <= chachaOut[159:152];
                //end else if (outputIndex ==20) begin 
                //    uart_txbyte <= chachaOut[167:160];
                //end else if (outputIndex ==21) begin 
                //    uart_txbyte <= chachaOut[175:168];
                //end else if (outputIndex ==22) begin 
                //    uart_txbyte <= chachaOut[183:176];
                //end else if (outputIndex ==23) begin 
                //    uart_txbyte <= chachaOut[191:184];
                //end else if (outputIndex ==24) begin 
                //    uart_txbyte <= chachaOut[199:192];
                //end else if (outputIndex ==25) begin 
                //    uart_txbyte <= chachaOut[207:200];
                //end else if (outputIndex ==26) begin 
                //    uart_txbyte <= chachaOut[215:208];
                //end else if (outputIndex ==27) begin 
                //    uart_txbyte <= chachaOut[223:216];
                //end else if (outputIndex ==28) begin 
                //    uart_txbyte <= chachaOut[231:224];
                //end else if (outputIndex ==29) begin 
                //    uart_txbyte <= chachaOut[239:232];
                //end else if (outputIndex ==30) begin 
                //    uart_txbyte <= chachaOut[247:240];
                //end else if (outputIndex ==31) begin 
                //    uart_txbyte <= chachaOut[255:248];
                //end else if (outputIndex ==32) begin 
                //    uart_txbyte <= chachaOut[263:256];
                //end else if (outputIndex ==33) begin 
                //    uart_txbyte <= chachaOut[271:264];
                //end else if (outputIndex ==34) begin 
                //    uart_txbyte <= chachaOut[279:272];
                //end else if (outputIndex ==35) begin 
                //    uart_txbyte <= chachaOut[287:280];
                //end else if (outputIndex ==36) begin 
                //    uart_txbyte <= chachaOut[295:288];
                //end else if (outputIndex ==37) begin 
                //    uart_txbyte <= chachaOut[303:296];
                //end else if (outputIndex ==38) begin 
                //    uart_txbyte <= chachaOut[311:304];
                //end else if (outputIndex ==39) begin 
                //    uart_txbyte <= chachaOut[319:312];
                //end else if (outputIndex ==40) begin 
                //    uart_txbyte <= chachaOut[327:320];
                //end else if (outputIndex ==41) begin 
                //    uart_txbyte <= chachaOut[335:328];
                //end else if (outputIndex ==42) begin 
                //    uart_txbyte <= chachaOut[343:336];
                //end else if (outputIndex ==43) begin 
                //    uart_txbyte <= chachaOut[351:344];
                //end else if (outputIndex ==44) begin 
                //    uart_txbyte <= chachaOut[359:352];
                //end else if (outputIndex ==45) begin 
                //    uart_txbyte <= chachaOut[367:360];
                //end else if (outputIndex ==46) begin 
                //    uart_txbyte <= chachaOut[375:368];
                //end else if (outputIndex ==47) begin 
                //    uart_txbyte <= chachaOut[383:376];
                //end else if (outputIndex ==48) begin 
                //    uart_txbyte <= chachaOut[391:384];
                //end else if (outputIndex ==49) begin 
                //    uart_txbyte <= chachaOut[399:392];
                //end else if (outputIndex ==50) begin 
                //    uart_txbyte <= chachaOut[407:400];
                //end else if (outputIndex ==51) begin 
                //    uart_txbyte <= chachaOut[415:408];
                //end else if (outputIndex ==52) begin 
                //    uart_txbyte <= chachaOut[423:416];
                //end else if (outputIndex ==53) begin 
                //    uart_txbyte <= chachaOut[431:424];
                //end else if (outputIndex ==54) begin 
                //    uart_txbyte <= chachaOut[439:432];
                //end else if (outputIndex ==55) begin 
                //    uart_txbyte <= chachaOut[447:440];
                //end else if (outputIndex ==56) begin 
                //    uart_txbyte <= chachaOut[455:448];
                //end else if (outputIndex ==57) begin 
                //    uart_txbyte <= chachaOut[463:456];
                //end else if (outputIndex ==58) begin 
                //    uart_txbyte <= chachaOut[471:464];
                //end else if (outputIndex ==59) begin 
                //    uart_txbyte <= chachaOut[479:472];
                //end else if (outputIndex ==60) begin 
                //    uart_txbyte <= chachaOut[487:480];
                //end else if (outputIndex ==61) begin 
                //    uart_txbyte <= chachaOut[495:488];
                //end else if (outputIndex ==62) begin 
                //    uart_txbyte <= chachaOut[503:496];
                //end else if (outputIndex ==63) begin 
                //    uart_txbyte <= chachaOut[511:504];
                end else begin
                    uart_txbyte <= 0;
                end
                */

                led2 <= 1;
			end

        //begin transmission
  	    end else if (UartStateCounter == 2) begin
		    uart_send_en <= 1;
		    uart_send <= 1;
		    UartStateCounter <= 3;
            led3 <= 1;

        //wait for transmission to start
	    end else if (UartStateCounter == 3) begin
		    if (uart_txed == 0) begin
			    UartStateCounter <= 4;
                led4 <= 1;
		    end

        //wait for transmission to end
  	    end else if (UartStateCounter == 4) begin
		    if (uart_txed == 1) begin
			    uart_send_en <= 0;
			    uart_send <= 0;
		        ledGreen <= 1;
		        UartStateCounter <= 0;
                //chachaOutCopy <= chachaOutCopy >> 8;
                //chachaOut <= chachaOut >> 8;
                outputIndex <= outputIndex + 1;

                if (outputIndex == 64) begin
                    //chachaOutCopy <= chachaOut;
                    outputIndex <= 0;
                end
		    end

        //init leds
	    end else if (UartStateCounter == 15) begin
			led3 <= 0;
			led0 <= 0;
			led1 <= 0;
			led2 <= 0;
			ledGreen <= 0;
            chachaStart <= 1;
			UartStateCounter <= 0;
	    end

end

endmodule
