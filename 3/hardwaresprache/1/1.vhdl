library ieee;
use ieee.std_logic_1164.all;

entity FullAdder is

--generic (width : integer := 1);

port(a		: in	std_logic; --std_logic_vector(width - 1 downto 0);
     b		: in	std_logic; --std_logic_vector(width - 1 downto 0);
     cin		: in 	std_logic; --std_logic;
					         
     s 		: out	std_logic; --std_logic_vector(width - 1 downto 0);
     cout		: out	std_logic); --std_logic);

end FullAdder;

architecture Behavioral of FullAdder is
begin
	s <= (not a and not b and cin) or (not a and b and not cin) or (a and not b and not cin) or (a and b and cin);
	cout <= (a and b) or (cin and not a and b) or (cin and a and not b);
end Behavioral;
