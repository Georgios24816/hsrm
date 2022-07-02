library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart8n1_tr is
    port 
    (
        dummyOut: out unsigned(0 downto 0)
    );
end uart8n1_tr;

architecture rtl of uart8n1_tr is
begin
    dummyOut <= "1";
end rtl;