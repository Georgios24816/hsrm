INSERT INTO bestellung (be_nr, zeitstempel, p_nr, bestellweg) 
VALUES ("BE-0070", "2020-1-1 18:04:00.0", (SELECT p.p_nr FROM person p WHERE p.vorname="Jürgen" AND p.name="Müller"), "Internet");

INSERT INTO bestellung_beinhaltet_medienartikel (a_nr, be_nr, anzahl, einzelpreis)
VALUES 	((SELECT m.a_nr FROM medienartikel m, dvd d WHERE m.titel="Pirates of the Caribbean" AND m.a_nr=d.a_nr), 
		"BE-0070", 
		1, 
		(SELECT DISTINCT a.preis FROM angebot a WHERE a.a_nr=(SELECT DISTINCT m.a_nr FROM medienartikel m, dvd d WHERE m.titel="Pirates of the Caribbean" AND m.a_nr=d.a_nr) GROUP BY a.a_nr))
