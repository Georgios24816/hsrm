---------------------------------------------------------------------------------------------------
--
-- Titel: Control Unit für HBS
-- Autor: Georgios Markou
-- Datum: 23.12.2020 13:36
--
---------------------------------------------------------------------------------------------------

library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;
library work;
	use work.hadescomponents.all;
	
entity control is
	port (
		-- common
		clk 		: in  std_logic;
		reset		: in  std_logic;
		
		-- control inputs
		inop		: in  std_logic;
		outop		: in  std_logic;
		loadop		: in  std_logic;
		storeop		: in  std_logic;
		dpma		: in  std_logic;
		epma		: in  std_logic;
		xack		: in  std_logic;
		xpresent	: in  std_logic;
		dmembusy	: in  std_logic;
		
		-- control outputs
		loadir		: out std_logic;
		regwrite	: out std_logic;
		pcwrite		: out std_logic;
		pwrite		: out std_logic;
		xread		: out std_logic;
		xwrite		: out std_logic;
		xnaintr		: out std_logic
	);
end control;

architecture rtl of control is
type T_State is --Zustaende der State Machine der Control Unit
(
T_IFetch, T_IDecode, T_Alu, T_IORead, T_IOWrite, T_MemRead, 
T_MemWrite, T_WriteBack, T_XBusNaIntr
);

signal currentState : T_State := T_IFetch; --Startzustand

begin
	process (clk, reset)
	begin
		if (rising_edge(clk)) then
			if (currentState = T_IFetch) then 
				--Instruktion holen
				if (loadir = '1') then
				--loadir muss gesetzt sein um die naechste
				--Instruktion zu holen
				currentState = T_IDecode; --naechster Zustand
				end if;
			end if;

			elsif (currentState = T_IDecode) then
				--Instruktion dekodieren
			end if;
		end if;

	end process;

end rtl;
