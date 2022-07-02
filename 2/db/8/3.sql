CREATE PROCEDURE nachbestellung_zusammenstellen()
DECLARE @ArtikelCursor CURSOR;
DECLARE @ArtikelNr VARCHAR(13);
DECLARE @ArtikelPreis INT;
DECLARE @LagerAlt INT;
DECLARE @Lager INT;
DECLARE @Nachbestellen INT;
BEGIN
	SET @ArtikelCursor = CURSOR FOR
	SELECT a_nr FROM medienartikel;

	OPEN @ArtikelCursor
	FETCH NEXT FROM @ArtikelCursor
	INTO @ArtikelNr

	WHILE @@FETCH_STATUS = 0
	BEGIN
		SET @ArtikelPreis = (SELECT DISTINCT preis FROM angebot WHERE a_nr=@ArtikelNr)
		SET @LagerAlt = (SELECT bestand FROM lagerbestand WHERE a_nr=@ArtikelNr)
		SET @Lager = IF(@ArtikelPreis > 6, 5, 10)
		SET @Nachbestellen = Lager - LagerAlt

		UPDATE lagerbestand SET bestand = Lager WHERE a_nr=@ArtikelNr
		UPDATE nachbestellen SET best_anzahl = Nachbestellen WHERE a_nr=@ArtikelNr

		DELETE FROM nachbestellen WHERE best_anzahl <= 0; --ungültige Bestellung löschen

		FETCH NEXT FROM @ArtikelCursor
		INTO @ArtikelNr
	END;

	CLOSE @ArtikelCursor;
	DEALLOCATE @ArtikelCursor;
END

