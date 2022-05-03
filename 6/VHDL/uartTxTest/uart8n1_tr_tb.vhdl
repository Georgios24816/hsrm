library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart8n1_tr_tb is
        port
        (
            hwclk     : in      std_ulogic;
            ftdi_tx   : out     unsigned(0 downto 0);
            ftdi_rx   : out     unsigned(0 downto 0)
        );
end uart8n1_tr_tb;

architecture test of uart8n1_tr_tb is
    component uart8n1_tx
        port 
        (
            clk     : in    std_ulogic;
            en      : in    unsigned(0 downto 0);
            txByte  : in    unsigned(7 downto 0);
            sendData: in    unsigned(0 downto 0);
    
            txDone  : out   unsigned(0 downto 0);
            tx      : out   unsigned(0 downto 0)
        );
    end component;

    component uart8n1_rx2
        port 
        (
            clk     : in    std_ulogic;
            en      : in    unsigned(0 downto 0);
            rxIn    : in    unsigned(0 downto 0);
            rxDv    : out   unsigned(0 downto 0);
            isIdle  : out   unsigned(0 downto 0);
            rxByte  : out   unsigned(7 downto 0)
        );
    end component;

--uart8n1_rx2
signal clk_9600     : std_ulogic            := '0';
signal uart_txbyte  : unsigned(7 downto 0)  := "00000000";
signal uart_send    : unsigned(0 downto 0)  := "0";
signal uart_receive : unsigned(0 downto 0)  := "0";
signal uart_txed    : unsigned(0 downto 0)  := "0";
signal uart_tx      : unsigned(0 downto 0)  := "0";
signal cntr_9600    : unsigned(31 downto 0) := (others => '0');
signal period9600   : unsigned(31 downto 0) := to_unsigned(625, 32);

signal ledGreenSignal : unsigned(0 downto 0) := "0";
signal cntr_32    : unsigned(31 downto 0) := (others => '0');

signal rxInSignal : unsigned(0 downto 0) := "0";
signal rxDvSignal : unsigned(0 downto 0) := "0";

signal uartActive : unsigned(0 downto 0) := "0";
signal txIdleSignal : unsigned(0 downto 0) := "0";
signal rxIdleSignal : unsigned(0 downto 0) := "0";

signal txEnable: unsigned(0 downto 0) := '0';
signal rxEnable: unsigned(0 downto 0) := '0';
signal txInUse: unsigned(0 downto 0) := '0';
signal rxInUse: unsigned(0 downto 0) := '0';

--uart8n1_rx2

--uart8n1_tx
--signal uart_txbyte  : unsigned(7 downto 0)  := "00000000";
--signal uart_send    : unsigned(0 downto 0)  := "0";
--signal uart_txed    : unsigned(0 downto 0)  := "0";
--signal uart_tx      : unsigned(0 downto 0)  := "0";
--signal cntr_9600    : unsigned(31 downto 0) := (others => '0');
--signal period9600   : unsigned(31 downto 0) := to_unsigned(625, 32);
--
--signal cntr_32    : unsigned(31 downto 0) := (others => '0');
--uart8n1_tx

signal uart_txbyteCopy  : unsigned(7 downto 0)  := "00000000";

begin
    uart_receive <= not uart_send;

    uart8n1_tx_Instance: uart8n1_tx 
    port map
    (
        clk => clk_9600,
        en => txEnable,
        txByte => uart_txbyteCopy,
        sendData => uart_send,
        isIdle => txIdleSignal,
        txDone => uart_txed,
        tx => ftdi_tx
    );

    uart8n1_rx2_Instance: uart8n1_rx2 
    port map
    (
        clk => hwclk,
        en => rxEnable,
        rxIn => ftdi_rx,
        rxDv => uart_receive,
        isIdle => rxIdleSignal,
        rxByte => uart_txbyte
    );

    process (hwclk)
        constant State_idle         : unsigned(3 downto 0)  := "0000";
        constant State_txStarted    : unsigned(3 downto 0)  := "0001";
        constant State_rxStarted    : unsigned(3 downto 0)  := "0010";

        variable State              : unsigned(3 downto 0)  := State_idle;   

    begin
        if rising_edge(hwclk) then
            cntr_9600 <= cntr_9600 + 1;
            cntr_32 <= cntr_32 + 1;

            if (cntr_9600 = period9600) then
                clk_9600 <= not clk_9600;
                cntr_9600 <= to_unsigned(0, 32);
            end if;

            if (uartActive = "1") then
                if (State = State_idle and uart_send = "1" and rxIdleSignal = "1" and txIdleSignal = "1") then
                    txEnable <= "1";
                    uart_txbyteCopy <= uart_txbyte;
                    State := State_txStarted;

                elsif (State = State_txStarted) then
                    if (txIdleSignal = "0") then
                        State := State_idle;
                    end if;

                elsif (state = State_idle and uart_receive = "0" and rxIdleSignal = "1" and txIdleSignal = "1") then
                    rxEnable <= "1";
                    State := State_idle;
                    
                elsif (state = State_rxStarted) then
                    if (rxIdleSignal = "1") then
                        State := State_idle;
                    end if;
                end if;
            end if;

        end if;
    end process;

end test;