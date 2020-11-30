CREATE TABLE lagerbestand (
	a_nr INT NOT NULL,
	bestand INT NOT NULL,
	PRIMARY KEY (a_nr),
	FOREIGN KEY (a_nr) REFERENCES medienartikel (a_nr)
) DEFAULT CHARSET=utf8;
