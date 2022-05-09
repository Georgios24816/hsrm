library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity chacha_qr is
    port 
    (
        clk : in    std_ulogic;

        a   : in    unsigned(31 downto 0);
        b   : in    unsigned(31 downto 0);
        c   : in    unsigned(31 downto 0);
        d   : in    unsigned(31 downto 0);

        a2  : out   unsigned(31 downto 0);
        b2  : out   unsigned(31 downto 0);
        c2  : out   unsigned(31 downto 0);
        d2  : out   unsigned(31 downto 0);

        done: out   unsigned(0 downto 0)
    );
end chacha_qr;

architecture behave of chacha_qr is
signal a0Signal: unsigned(31 downto 0);
signal b0Signal: unsigned(31 downto 0);
signal c0Signal: unsigned(31 downto 0);
signal d0Signal: unsigned(31 downto 0);

signal a1Signal: unsigned(31 downto 0);
signal b1Signal: unsigned(31 downto 0);
signal c1Signal: unsigned(31 downto 0);
signal d1Signal: unsigned(31 downto 0);

signal a2Signal: unsigned(31 downto 0);
signal b2Signal: unsigned(31 downto 0);
signal c2Signal: unsigned(31 downto 0);
signal d2Signal: unsigned(31 downto 0);

signal a3Signal: unsigned(31 downto 0);
signal b3Signal: unsigned(31 downto 0);
signal c3Signal: unsigned(31 downto 0);
signal d3Signal: unsigned(31 downto 0);

signal aSignal: unsigned(31 downto 0);
signal bSignal: unsigned(31 downto 0);
signal cSignal: unsigned(31 downto 0);
signal dSignal: unsigned(31 downto 0);

begin
    a2 <= aSignal;
    b2 <= bSignal;
    c2 <= cSignal;
    d2 <= dSignal;

    process (clk)
        variable state: unsigned(1 downto 0) := "00";
        variable index: unsigned(4 downto 0) := "00000";
    begin
        if (rising_edge(clk)) then
            if (state = "00") then
                if (index = "00000") then
                    a0Signal <= a + b;
                    b0Signal <= b;
                    c0Signal <= c;
                    d0Signal <= (d xor (a + b)) sll 16;

                else 
                    a0Signal <= aSignal + bSignal;
                    b0Signal <= bSignal;
                    c0Signal <= cSignal;
                    d0Signal <= (dSignal xor (aSignal + bSignal)) sll 16;
                end if;
                done <= "0";
                state := "01";

            elsif (state = "01") then
                --c += d; b ^= c; b <<<= 12;
                a1Signal <= a0Signal;
                d1Signal <= d0Signal;
                c1Signal <= c0Signal + d0Signal;
                b1Signal <= (b0Signal xor (c0Signal + d0Signal)) sll 12;
                state := "10";

            elsif (state = "10") then
                --a += b; d ^= a; d <<<= 8;
                b2Signal <= b1Signal;
                c2Signal <= c1Signal;
                a2Signal <= a1Signal + b1Signal;
                d2Signal <= (d1Signal xor (a1Signal + b1Signal)) sll 8;
                state := "11";

            else
                --c += d; b ^= c; b <<<= 7;
                a3Signal <= a2Signal;
                d3Signal <= d2Signal;
                c3Signal <= c2Signal + d2Signal;
                b3Signal <= (b2Signal xor (c2Signal + d2Signal)) sll 7;


                aSignal <= a3Signal;
                dSignal <= d3Signal;
                cSignal <= c3Signal;
                bSignal <= b3Signal;

                done <= "1";
                if (index = "10100" and state = "11") then
                    done <= "1";
                    index := "00000";
                
                else
                    index := index + 1;
                end if;

                state := "00";
            end if;
        end if;
    end process;
end behave;