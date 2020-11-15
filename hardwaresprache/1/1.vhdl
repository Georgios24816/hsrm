library ieee;
use ieee.std_logic_1164.all;

entity Volladdierer is

--generic (width : integer := 1);

port(in1		: in	std_logic--std_logic_vector(width - 1 downto 0);
     in2		: in	std_logic--std_logic_vector(width - 1 downto 0);
     carryIn	: in 	std_logic--std_logic;
						
     result 	: out	std_logic--std_logic_vector(width - 1 downto 0);
     carryOut	: out	std_logic--std_logic);

end Volladdierer

architecture VolladdiererArch of Volladdierer is
begin
	result <= (not in1 and not in2 and carryIn) or (not in1 and in2 and not carryIn) or (in1 and not in2 and not carryIn) or (in1 and in2 and carryIn);
	carryOver <= (in1 and in2) or (carryIn and not in1 and in2) or (carryIn and in1 and not in2);
end VolladdiererArch;
