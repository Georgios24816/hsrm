library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart8n1_rx2 is
    port 
    (
        clk     : in    std_ulogic;
        rxIn    : in    unsigned(0 downto 0);

        rxDv    : out   unsigned(0 downto 0);
        rxByte  : out   unsigned(7 downto 0)
    );

subtype st_logicBit is std_ulogic;
subtype st_bit is unsigned(0 downto 0);
subtype st_byte is unsigned(7 downto 0);
subtype state_t is unsigned(2 downto 0);
end uart8n1_rx2;

architecture behave of uart8n1_rx2 is
signal rxByteSignal : st_byte := (others => '0');
begin
    rxByte <= rxByteSignal;

    process (clk)
    constant StateIdle      : state_t   := "000";
    constant StateStartRx   : state_t   := "001";
    constant StateRxing     : state_t   := "010";
    constant StateRxDone    : state_t   := "011";
    constant StateRxCleanup : state_t   := "100";

    variable state          : state_t   := StateIdle;
    variable bufTx          : st_byte   := "00000000";
    variable bitIndex       : integer   := 0;
    variable posEdgeCounter : unsigned(31 downto 0) := (others => '0');

    variable g_CLKS_PER_BIT : integer := 1250;

    begin
        if (rising_edge(clk)) then
            if (state = StateIdle) then
                rxDv <= "0";
                posEdgeCounter := to_unsigned(0, 32);
                bitIndex := 0;

                if (rxIn = "0") then
                    state := StateStartRx;
                else
                    state := StateIdle;
                end if;

            elsif (state = StateStartRx) then
                if (posEdgeCounter = to_unsigned((g_CLKS_PER_BIT-1)/2, 32)) then
                    if (rxIn = "0") then
                        posEdgeCounter := (others => '0');
                        state := StateRxing;
                    else
                        state := StateIdle;
                    end if;

                else
                    posEdgeCounter := posEdgeCounter + 1;
                    state := StateStartRx;
                end if;

            elsif (state = StateRxing) then
                if (posEdgeCounter < g_CLKS_PER_BIT-1) then
                    posEdgeCounter := posEdgeCounter + 1;
                    state := StateRxing;

                else
                    posEdgeCounter := to_unsigned(0, 32);

                    if (rxIn = "1") then
                        rxByteSignal(bitIndex) <= '1';
                    else
                        rxByteSignal(bitIndex) <= '0';
                    end if;

                    if (bitIndex < 7) then
                        bitIndex := bitIndex + 1;
                        state := StateRxing;

                    else
                        bitIndex := 0;
                        state := StateRxDone;
                    end if;
                end if;

            elsif (state = StateRxDone) then
                if (posEdgeCounter < to_unsigned(g_CLKS_PER_BIT - 1, 32)) then
                    posEdgeCounter := posEdgeCounter + 1;
                    state := StateRxDone;

                else
                    rxDv <= "1";
                    posEdgeCounter := to_unsigned(0, 32);
                    state := StateRxCleanup;
                end if;

            elsif (state = StateRxCleanup) then
                state := StateIdle;
                rxDv <= "0";

            end if;

        end if;
    end process;

end behave;