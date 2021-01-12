library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;

library work;
	use work.all;
	
entity pclogic2_tb is
end pclogic2_tb;

architecture sim of pclogic2_tb is
clk 		: std_logic;
reset		: std_logic;
pcwrite		: std_logic;
pccontr		: std_logic_vector(5 downto 0);
ov			: std_logic;
zero		: std_logic;
intr		: std_logic;
pcnew		: std_logic_vector(11 downto 0);
isra		: std_logic_vector(11 downto 0);
isrr		: std_logic_vector(11 downto 0)
pcakt		: std_logic_vector(11 downto 0);
pcinc		: std_logic_vector(11 downto 0);
pcnext		: std_logic_vector(11 downto 0)

begin


end sim;
