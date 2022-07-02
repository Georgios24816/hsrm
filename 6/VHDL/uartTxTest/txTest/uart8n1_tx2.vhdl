library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart8n1_tx2 is
    port 
    (
        clk     : in    std_ulogic;
        en      : in    unsigned(0 downto 0);
        txByte  : in    unsigned(7 downto 0);
        sendData: in    unsigned(0 downto 0);

        isIdle  : out   unsigned(0 downto 0);
        tx      : out   unsigned(0 downto 0)
    );
end uart8n1_tx2;

architecture behave of uart8n1_tx2 is
    component uart8n1_tx
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
    end component;



signal txByteSignal: unsigned(7 downto 0) := "00000000";
signal txDoneSignal: unsigned(0 downto 0) := "1";

begin
    uart8n1_txInstance: uart8n1_tx
    port map 
    (
        clk => clk,
        en => en,
        txByte => txByteSignal,
        sendData => sendData,
        isIdle => isIdle,
        txDone => txDoneSignal,
        tx => tx
    );

    txByteSignal <= txByte;
    

--    process (clk)
--        variable state: unsigned(2 downto 0) := "000";
--    begin
--        if (rising_edge(clk)) then
--            txByteSignal <= txByte;
--            --if (txDoneSignal = "1" and done = "1") then
--            --    if (state = "000") then
--            --        txByteSignal <= a2(7 downto 0);
--            --        state := state + 1;
--            --    
--            --    elsif (state = "001") then
--            --        txByteSignal <= a2(15 downto 8);
--            --        state := state + 1;
----
--            --    elsif (state = "010") then
--            --        txByteSignal <= a2(23 downto 16);
--            --        state := state + 1;
----
--            --    else
--            --        txByteSignal <= a2(31 downto 24);
--            --        state := "000";
--            --    end if;
--            --end if;
--        end if;
--    end process;

end behave;