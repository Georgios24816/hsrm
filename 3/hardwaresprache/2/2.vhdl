library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity AddSub1 is

port(mode	: in 	std_logic; 
	 a		: in	std_logic;
     b		: in	std_logic;
     cin	: in 	std_logic;
					         
     s 		: out	std_logic;
     cout	: out	std_logic
	 );

end AddSub1;

architecture Behavioral of ADDSUB1 is
begin

	AddSub1 : process(mode, a, b, cin)
	begin
	
	if (mode = '1') then
		s <= (not a and not b and cin) or (not a and b and not cin) or (a and not b and not cin) or (a and b and cin);
		cout <= (a and b) or (cin and not a and b) or (cin and a and not b);
		
	elsif (mode = '0') then
		s <= a xor b xor cin;
		cout <= (not a and b) or not (a xor b) and cin;
	end if;
		
	end process AddSub1;
		
end Behavioral;



--entity AddSub is
--
--generic (N : integer := 1);
--
--port(mode	: in 	std_logic; 
--	 a		: in	std_logic_vector(N - 1 downto 0);
--     b		: in	std_logic_vector(N - 1 downto 0);
--     cin	: in 	std_logic;
--					         
--     s 		: out	std_logic_vector(N - 1 downto 0);
--     cout	: out	std_logic;
--	 );
--
--end AddSub;
--
--architecture Behavioral of AddSub is
--begin
--	if (mode = '1') then
--		s <= (not a and not b and cin) or (not a and b and not cin) or (a and not b and not cin) or (a and b and cin);
--		cout <= (a and b) or (cin and not a and b) or (cin and a and not b);
--	
--	elsif (mode = '0') then
--	
--end Behavioral;
