LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

PACKAGE ADDSUB_PACKAGE IS
	CONSTANT ADDSUB_IN_WIDTH  	: integer := 8;
	CONSTANT ADDSUB_OUT_WIDTH 	: integer := ADDSUB_IN_WIDTH + 1;
	CONSTANT ADDSUB_MODE_WIDTH	: integer := 1;

	CONSTANT ADDSUB_IN_VALUE_L 	: integer := -(2 ** (ADDSUB_IN_WIDTH - 1));
	CONSTANT ADDSUB_IN_VALUE_H 	: integer := (-ADDSUB_IN_VALUE_L) - 1;
	
	CONSTANT ADDSUB_OUT_VALUE_L : integer := -(2 ** (ADDSUB_OUT_WIDTH - 1));
	CONSTANT ADDSUB_OUT_VALUE_H : integer := (-ADDSUB_OUT_VALUE_L) - 1;

	SUBTYPE ADDSUB_IN_VALUE  IS integer RANGE ADDSUB_IN_VALUE_L TO ADDSUB_IN_VALUE_H;
	SUBTYPE ADDSUB_OUT_VALUE IS integer RANGE ADDSUB_OUT_VALUE_L TO ADDSUB_OUT_VALUE_H;
	SUBTYPE ADDSUB_MODE		 IS integer RANGE 0 TO 1;
END my_package;

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE work.ADDSUB_PACKAGE.ALL;

ENTITY AddSub IS
	PORT
	(
		a		: 		IN ADDSUB_IN_VALUE;
		b		: 		IN ADDSUB_IN_VALUE;
		mode	: 		IN ADDSUB_MODE;
		
		result	: 		OUT ADDSUB_OUT_VALUE;
	);
END AddSub;

ARCHITECTURE Behavioral OF AddSub IS
BEGIN
	PROCESS (a, b, mode)
	BEGIN
		result <= 	a + b when mode = "1" else
					a - b when mode = "0";
	END PROCESS;
END Behavioral;