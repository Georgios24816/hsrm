---------------------------------------------------------------------------------------------------
--
-- Titel:    
-- Autor: Georgios 
-- Datum:    
--
---------------------------------------------------------------------------------------------------

library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;
--library work;
--	use work.hadescomponents.all;
	
entity haregs is
	port (
		-- common
		clk 		: in  std_logic;
		reset		: in  std_logic;
		
		-- write port
		regwrite	: in  std_logic;
		wopadr		: in  std_logic_vector(2 downto 0);
		wop			: in  std_logic_vector(31 downto 0);
		
		-- read port A
		aopadr		: in  std_logic_vector(2 downto 0);
		aop			: out std_logic_vector(31 downto 0);
		
		-- read port B
		bopadr		: in  std_logic_vector(2 downto 0);
		bop			: out std_logic_vector(31 downto 0)
	);
end haregs;

architecture rtl of haregs is

type haregs_t is array (7 downto 0) of std_logic_vector(31 downto 0);
signal haregsSig : haregs_t;

begin

	PROCESS (clk, reset)
	BEGIN
		if (reset = '1') then
			
		end if;
	
	    if (rising_edge(clk)) then
	        if (regwrite = '1') then
	            ;
	        end if;
	    end if;
	
	END PROCESS;
	
end rtl;