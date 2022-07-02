CREATE FUNCTION fakultaet (x INT)
	RETURNS INT DETERMINISTIC
	BEGIN
		DECLARE ergebnis INT;
		DECLARE i INT;
		SET ergebnis = 1;
		SET i = 1;

		WHILE ( i <= x)
		BEGIN
			SET ergebnis = ergebnis * i;
			SET i = i + 1;
		END

		RETURN fakulaet
	END;
