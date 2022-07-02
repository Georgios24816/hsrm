CREATE TRIGGER tr_nachbestellung
AFTER UPDATE ON lagerbestand
FOR EACH ROW
UPDATE lagerbestand SET bestand = bestand + NEW.anzahl
