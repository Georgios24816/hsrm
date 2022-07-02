library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart8n1_tx is
    port 
    (
        clk     : in    std_ulogic;
        en      : in    unsigned(0 downto 0);
        txByte  : in    unsigned(7 downto 0);
        sendData: in    unsigned(0 downto 0);

        isIdle  : out   unsigned(0 downto 0);
        txDone  : out   unsigned(0 downto 0);
        tx      : out   unsigned(0 downto 0)
    );

subtype st_logicBit is std_ulogic;
subtype st_bit is unsigned(0 downto 0);
subtype st_byte is unsigned(7 downto 0);
subtype state_t is unsigned(1 downto 0);
end uart8n1_tx;

architecture behave of uart8n1_tx is
signal txBit: st_bit    := "1";
--signal bufTx: st_byte   := "00000000";

signal isidleSignal: unsigned(0 downto 0) := "1";
signal isDoneSignal: unsigned(0 downto 0) := "1";

begin
    tx <= txBit;
    isIdle <= isidleSignal;
    txDone <= isDoneSignal;

    --bufTx <= txByte;

    process (clk)
    
    constant StateIdle      : state_t   := "00";
    constant StateStartTx   : state_t   := "01";
    constant StateTxing     : state_t   := "10";
    constant StateTxDone    : state_t   := "11";

    variable state          : state_t   := StateIdle;
    variable bufTx: st_byte   := "00000000";
    variable bitsSent       : st_byte   := "00000000";
    --variable txBit          : _bit    := "1";*/

    begin 
        if (rising_edge(clk) and en = "1") then
            if (state = StateIdle) then
                isidleSignal <= "1";
                if (sendData = "1" and state = StateIdle) then
                    state := StateStartTx;
                    bufTx := txByte;
                    isDoneSignal <= "0";

                elsif (state = StateIdle) then
                    txBit <= "1";
                    isDoneSignal <= "0";
                end if;

            elsif (state = StateStartTx) then
                isidleSignal <= "0";
                if (state = StateStartTx) then
                    txBit <= "0";
                    state := StateTxing;
                end if;

            elsif (state = StateTxing) then
                isidleSignal <= "0";
                if (state = StateTxing and bitsSent < to_unsigned(8, 8)) then
                    txBit <= bufTx(0 downto 0);
                    bufTx := shift_right(bufTx, 1);
                    bitsSent := bitsSent + 1;
            
                elsif (state = StateTxing) then
                    txBit <= "1";
                    bitsSent := "00000000";
                    state := StateTxDone;
                end if;

            elsif (state = StateTxDone) then
                isidleSignal <= "0";
                if (state = StateTxDone) then
                    isDoneSignal <= "1";
                    state := StateIdle;
                end if;
            end if;
        end if;

    end process;
end behave;