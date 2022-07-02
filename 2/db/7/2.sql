CREATE TRIGGER aufgabe2 
	BEFORE UPDATE ON bestellung_beinhaltet_medienartikel --oder INSERT
	--DECLARE @maxAnzahl INT
	--SET @maxAnzahl = (SELECT bestand FROM lagerbestand WHERE a_nr = NEW.a_nr)
	FOR EACH ROW
	SET NEW.anzahl = IF(NEW.anzahl > (SELECT bestand FROM lagerbestand WHERE a_nr = NEW.a_nr), 
					(SELECT bestand FROM lagerbestand WHERE a_nr = NEW.a_nr), 
					NEW.anzahl)
	
