CREATE TRIGGER aufgabe3_insert
	AFTER INSERT ON TABLE bestellung
	FOR EACH ROW
	UPDATE lagerbestand SET bestand = bestand - NEW.anzahl 
	WHERE a_nr = NEW.a_nr;

CREATE TRIGGER aufgabe3_update
	AFTER UPDATE ON TABLE bestellung
	FOR EACH ROW
	UPDATE lagerbestand SET bestand = bestand - NEW.anzahl 
	WHERE a_nr = NEW.a_nr

CREATE TRIGGER aufgabe3_delete
	AFTER DELETE ON TABLE bestellung
	FOR EACH ROW
	UPDATE lagerbestand SET bestand = bestand + NEW.anzahl 
	WHERE a_nr = NEW.a_nr
