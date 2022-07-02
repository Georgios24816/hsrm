CREATE TABLE `medienartikel` (
  `a_nr` varchar(13) NOT NULL,
  `titel` varchar(100) NOT NULL,
  `jahr` int(4) NOT NULL,
  PRIMARY KEY (`a_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `person` (
  `p_nr` varchar(13) NOT NULL,
  `vorname` varchar(25) NOT NULL,
  `name` varchar(30) NOT NULL,
  PRIMARY KEY (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `auszeichnung` (
  `typ` varchar(10) NOT NULL,
  `jahr` int(4) NOT NULL,
  PRIMARY KEY (`typ`,`jahr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `verlag` (
  `verlagname` varchar(13) NOT NULL,
  `ort` varchar(30) NOT NULL,
  PRIMARY KEY (`verlagname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `musikstueck` (
  `id` varchar(13) NOT NULL,
  `dauer` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `schauspieler` (
  `p_nr` varchar(13) NOT NULL,
  `mgr` varchar(25) NOT NULL,
  PRIMARY KEY (`p_nr`),
  UNIQUE KEY `mgr` (`mgr`),
  FOREIGN KEY (`p_nr`) REFERENCES `person` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `regisseur` (
  `p_nr` varchar(13) NOT NULL,
  `studio` varchar(30) NOT NULL,
  PRIMARY KEY (`p_nr`),
  UNIQUE KEY `studio` (`studio`),
  FOREIGN KEY (`p_nr`) REFERENCES `person` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `buchautor` (
  `p_nr` varchar(13) NOT NULL,
  `agent` varchar(25) NOT NULL,
  PRIMARY KEY (`p_nr`),
  FOREIGN KEY (`p_nr`) REFERENCES `person` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `musiker` (
  `p_nr` varchar(13) NOT NULL,
  `impressario` varchar(25) NOT NULL,
  PRIMARY KEY (`p_nr`),
  FOREIGN KEY (`p_nr`) REFERENCES `person` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `kunde` (
  `p_nr` varchar(13) NOT NULL,
  `k_nr` varchar(13) NOT NULL,
  `adresse` varchar(50) NOT NULL,
  `typ` varchar(10) NOT NULL,
  `zahlungsart` varchar(10) NOT NULL,
  PRIMARY KEY (`p_nr`),
  UNIQUE KEY `k_nr` (`k_nr`),
  FOREIGN KEY (`p_nr`) REFERENCES `person` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `bestellung` (
  `be_nr` varchar(13) NOT NULL,
  `zeitstempel` timestamp NOT NULL,
  `p_nr` varchar(13) NOT NULL,
  `bestellweg` varchar(8) NOT NULL,
  PRIMARY KEY (`be_nr`),
  FOREIGN KEY (`p_nr`) REFERENCES `kunde` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `elektronischerartikel` (
  `a_nr` varchar(13) NOT NULL,
  `dateityp` varchar(15) NOT NULL,
  `dateigroesse` int(11) NOT NULL,
  `kompression` int(11) NOT NULL,
  PRIMARY KEY (`a_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `medienartikel` (`a_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `buch` (
  `a_nr` varchar(13) NOT NULL,
  `sprache` varchar(5) NOT NULL,
  `auflage` int(11) NOT NULL,
  `verlagname` varchar(30) NOT NULL,
  `isbn` varchar(29) NOT NULL,
  `kategorie` varchar(12) NOT NULL,
  PRIMARY KEY (`a_nr`),
  UNIQUE KEY `isbn` (`isbn`),
  FOREIGN KEY (`a_nr`) REFERENCES `medienartikel` (`a_nr`),
  FOREIGN KEY (`verlagname`) REFERENCES `verlag` (`verlagname`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `tontraeger` (
  `a_nr` varchar(13) NOT NULL,
  `genre` varchar(20) NOT NULL,
  `typ` varchar(10) NOT NULL,
  PRIMARY KEY (`a_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `medienartikel` (`a_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `film` (
  `a_nr` varchar(13) NOT NULL,
  `laufzeit` int(11) NOT NULL,
  `regisseur` varchar(13) NOT NULL,
  `produzent` varchar(25) NOT NULL,
  PRIMARY KEY (`a_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `medienartikel` (`a_nr`),
  FOREIGN KEY (`regisseur`) REFERENCES `regisseur` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `video` (
  `a_nr` varchar(13) NOT NULL,
  `sprache` varchar(5) NOT NULL,
  PRIMARY KEY (`a_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `film` (`a_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `dvd` (
  `a_nr` varchar(13) NOT NULL,
  `annotation` varchar(50) DEFAULT NULL,
  `regionalcode` int(11) NOT NULL,
  `soundmix` varchar(10) DEFAULT NULL,
  `sprache` varchar(5) NOT NULL,
  PRIMARY KEY (`a_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `film` (`a_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `person_erhaelt_auszeichnung` (
  `typ` varchar(10) NOT NULL,
  `jahr` int(4) NOT NULL,
  `p_nr` varchar(13) NOT NULL,
  PRIMARY KEY (`typ`,`jahr`,`p_nr`),
  FOREIGN KEY (`typ`,`jahr`) REFERENCES `auszeichnung` (`typ`,`jahr`),
  FOREIGN KEY (`p_nr`) REFERENCES `person` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `buch_hat_buchautor` (
  `a_nr` varchar(13) NOT NULL,
  `p_nr` varchar(13) NOT NULL,
  PRIMARY KEY (`a_nr`,`p_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `buch` (`a_nr`),
  FOREIGN KEY (`p_nr`) REFERENCES `buchautor` (`p_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `schauspieler_spielt_in_film` (
  `p_nr` varchar(13) NOT NULL,
  `a_nr` varchar(13) NOT NULL,
  PRIMARY KEY (`p_nr`,`a_nr`),
  FOREIGN KEY (`p_nr`) REFERENCES `schauspieler` (`p_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `film` (`a_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `tontraeger_enthaelt_musikstueck` (
  `a_nr` varchar(13) NOT NULL, 
  `id` varchar(13) NOT NULL,
  PRIMARY KEY (`a_nr`,`id`),
  FOREIGN KEY (`a_nr`) REFERENCES `tontraeger` (`a_nr`),
  FOREIGN KEY (`id`) REFERENCES `musikstueck` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `musiker_bespielt_tontraeger` (
  `p_nr` varchar(13) NOT NULL,
  `a_nr` varchar(13) NOT NULL,
  PRIMARY KEY (`p_nr`,`a_nr`),
  FOREIGN KEY (`p_nr`) REFERENCES `musiker` (`p_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `tontraeger` (`a_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `angebot` (
  `a_nr` varchar(13) NOT NULL,
  `preis` decimal(4,2) NOT NULL,
  `datum` date NOT NULL,
  PRIMARY KEY (`a_nr`, `datum`),
  FOREIGN KEY (`a_nr`) REFERENCES `medienartikel` (`a_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `bestellung_beinhaltet_medienartikel` (
  `a_nr` varchar(13) NOT NULL,
  `be_nr` varchar(13) NOT NULL,
  `anzahl` int(11) NOT NULL,
  `einzelpreis` decimal(4,2) NOT NULL,
  PRIMARY KEY (`a_nr`,`be_nr`),
  FOREIGN KEY (`a_nr`) REFERENCES `medienartikel` (`a_nr`),
  FOREIGN KEY (`be_nr`) REFERENCES `bestellung` (`be_nr`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO medienartikel VALUES
  ('0001-E', 'LaTeX in 21 Tagen','2004'),
  ('0003-E', 'Harry Potter III','1997'),
  ('0005-B', 'Der Schwarm','2004'),
  ('0006-B', 'LaTeX kurz und gut','2004'),
  ('0007-B', 'Simplify Your Life','2004'),
  ('0008-B', 'Ich bin dann mal weg...','2006'),
  ('0009-T', 'Back to Bedlam','2005'),
  ('0010-T', 'The Best of Michael Jackson','1990'),
  ('0011-T', 'Music','2000'),
  ('0012-T', 'Piece By Piece','2005'),
  ('0013-D', 'Pirates of the Caribbean','2004'),
  ('0014-D', 'Fluch der Karibik','2004'),
  ('0015-D', 'Die Reise der Pinguine','2006'),
  ('0016-D', 'Star Trek - 40 Years','2006'),
  ('0017-V', 'Pirates of the Caribbean','2004'),
  ('0018-V', 'The Da Vinci Code','2006'),
  ('0019-V', 'Kill Bill Colleaction','2004'),
  ('0020-V', 'Ocean´s Eleven','2002');

INSERT INTO person VALUES 
  ('P001', 'Hape', 'Kerkeling'),
  ('P002', 'Marion', 'Küstenmacher'),
  ('P003', 'Werner', 'Tiki Küstenbacher'),
  ('P004', 'Matthias K.', 'Dalheimer'),
  ('P005', 'Frank', 'Schätzing'),
  ('P006', 'Gore', 'Verbinski'),
  ('P007', 'Jerry', 'Bruckheimer'),
  ('P008', 'Luc', 'Jacquet'),
  ('P009', '', 'Madonna'),
  ('P010', 'Katy', 'Melua'),
  ('P011', 'Ron', 'Howard'),
  ('P012', 'Quentin', 'Terentino'),
  ('P013', 'Steven', 'Soderbergh'),
  ('P014', 'Gene', 'Roddenberry'),
  ('P015', 'Johnny', 'Depp'),
  ('P016', 'Orlando', 'Bloom'),
  ('P017', 'Keira', 'Knightley'),
  ('P018', 'Michael', 'Jackson'),
  ('P019', 'Jochen', 'Schmidt'),
  ('P020', 'Anna', 'Schulze'),
  ('P021', 'Jürgen', 'Müller'),
  ('P022', 'Katrin', 'Meyer'),
  ('P023', 'Jan', 'Kramer'),
  ('P024', 'Jeams', 'Blunt');

INSERT INTO verlag VALUES
  ('Fischer', 'Frankfurt'),
  ('O''Reilly', 'Köln'),
  ('Campus', 'Frankfurt, New York'),
  ('Malik', 'München'),
  ('Springer', 'Berlin');

INSERT INTO auszeichnung VALUES 
  ('Oscar', '2001'),
  ('Oscar', '2002'),
  ('Oscar', '2006'),
  ('Buchpreis', '2004'),
  ('Buchpreis', '2005'),
  ('Grammy', '2000');

INSERT INTO musikstueck VALUES
  ('MS-001', '184'),
  ('MS-002', '250'),
  ('MS-003', '175'),
  ('MS-004', '189'),
  ('MS-005', '201');

INSERT INTO elektronischerartikel VALUES
  ('0001-E', 'mp3', '2886', '12'),
  ('0003-E', 'pdf', '586', '55');

INSERT INTO buch VALUES
  ('0005-B', 'de_DE', '5', 'Fischer', '3-596164532', 'Belletristik'),
  ('0006-B', 'de_DE', '2', 'O''Reilly', '3-897215004', 'Sachbuch'),
  ('0007-B', 'de_DE', '1', 'Campus', '3-593375540', 'Sachbuch'),
  ('0008-B', 'de_DE', '21', 'Malik', '3-890293123', 'Sachbuch');

INSERT INTO tontraeger VALUES
  ('0009-T', 'Pop', 'CD'),
  ('0010-T', 'Pop', 'CD'),
  ('0011-T', 'Pop', 'CD'),
  ('0012-T', 'Pop', 'CD');

INSERT INTO schauspieler VALUES
  ('P015', 'Cristine Dembrowski'),
  ('P016', 'Alen Keshishian'),
  ('P017', 'Lindy King');

INSERT INTO regisseur VALUES
  ('P006', 'Walt Disney'),
  ('P008', 'Kinowelt Home Entertainment'),
  ('P014', 'Paramount'),
  ('P011', 'Sony Pictures'),
  ('P012', 'Touchstone'),
  ('P013', 'Warner Bros');

INSERT INTO film VALUES
  ('0013-D', '143', 'P006', 'Jerry Bruckheimer'),
  ('0014-D', '143', 'P006', 'Jerry Bruckheimer'),
  ('0015-D', '82', 'P008', 'Yves Darondeau'),
  ('0016-D', '1095', 'P014', 'Gene Roddenberry'),
  ('0017-V', '143', 'P006', 'Jerry Bruckheimer'),
  ('0018-V', '143', 'P011', 'Dan Brown'),
  ('0019-V', '237', 'P012', 'Lawrence Bender'),
  ('0020-V', '116', 'P013', 'Jerry Weintraub');

INSERT INTO video VALUES
  ('0017-V', 'en_US'),
  ('0018-V', 'de_DE'),
  ('0019-V', 'de_DE'),
  ('0020-V', 'de_DE');

INSERT INTO dvd VALUES
  ('0013-D', '-', '1', 'Dolby 5.1', 'en_US'),
  ('0014-D', '-', '2', 'Dolby 5.1', 'de_DE'),
  ('0015-D', '-', '2', 'Dolby 5.1', 'de_DE'),
  ('0016-D', '-', '2', 'Dolby 5.1', 'de_DE');

INSERT INTO buchautor VALUES
  ('P001', 'Amo Müller'),
  ('P002', '-'),
  ('P003', '-'),
  ('P004', '-'),
  ('P005', '-');

INSERT INTO musiker VALUES
  ('P024', 'Twenty First Artists'),
  ('P018', 'Raymone K. Bain'),
  ('P009', 'Marcus Behle'),
  ('P010', '-');

INSERT INTO kunde VALUES
  ('P019', '2006JS001', 'Musterstraße 1, 12345 Köln', 'mormal', 'Nachnahme'),
  ('P020', '2006AS001', 'Hauptstraße 6, 81232 München', 'mormal', 'Vorkasse'),
  ('P021', '2006JM001', 'Bahnhofstraße 9, 32421 Karlsruhe', 'mormal', 'Vorkasse'),
  ('P022', '2006KM001', 'Südstraße 3,34359 Münster', 'Premium', 'Nachnahme'),
  ('P023', '2006JK001', 'Nodergof 16 98762 Hamburg', 'mormal', 'Nachnahme');

INSERT INTO bestellung VALUES
  ('BE-0020', '2006-08-02 10:31', 'P020', 'Internet'),
  ('BE-0049', '2006-10-23 00:04', 'P019', 'Postweg'),
  ('BE-0010', '2006-07-23 16:13', 'P020', 'Internet'),
  ('BE-0023', '2006-08-11 09:41', 'P023', 'Internet'),
  ('BE-0056', '2006-11-15 18:04', 'P022', 'Kurier');

INSERT INTO person_erhaelt_auszeichnung VALUES 
  ('Oscar', '2001', 'P013'),
  ('Oscar', '2006', 'P008'),
  ('Oscar', '2002', 'P011'),
  ('Buchpreis', '2004', 'P005'),
  ('Buchpreis', '2005', 'P005'),
  ('Grammy', '2000', 'P009');

INSERT INTO buch_hat_buchautor VALUES
  ('0005-B','P005'),
  ('0006-B','P004'),
  ('0007-B','P003'),
  ('0007-B','P002'),
  ('0008-B','P001');

INSERT INTO schauspieler_spielt_in_film VALUES
  ('P015','0013-D'),
  ('P016','0013-D'),
  ('P017','0013-D'),
  ('P015','0014-D'),
  ('P016','0014-D'),
  ('P017','0014-D'),
  ('P015','0017-V'),
  ('P016','0017-V'),
  ('P017','0017-V');

INSERT INTO tontraeger_enthaelt_musikstueck VALUES
  ('0009-T','MS-001'),
  ('0009-T','MS-002'),
  ('0010-T','MS-003'),
  ('0010-T','MS-004'),
  ('0010-T','MS-005');

INSERT INTO angebot VALUES
  ('0001-E', '4.99', '2006-12-31'),
  ('0003-E', '0.50', '2006-12-31'),
  ('0006-B', '7.50', '2006-07-01'),
  ('0007-B', '11.90', '2006-08-01'),
  ('0007-B', '5.00', '2006-12-03'),
  ('0012-T', '14.90', '2006-06-01'),
  ('0013-D', '9.95', '2006-08-01'),
  ('0013-D', '12.95', '2006-10-05'),
  ('0017-V', '14.90', '2006-06-30'),
  ('0017-V', '10.00', '2006-12-03');

INSERT INTO bestellung_beinhaltet_medienartikel VALUES
  ('0001-E', 'BE-0020', '1', '4.99'),
  ('0007-B', 'BE-0020', '1', '11.90'),
  ('0010-T', 'BE-0049', '1', '14.90'),
  ('0017-V', 'BE-0010', '1', '14.90'),
  ('0008-B', 'BE-0010', '2', '8.90'),
  ('0012-T', 'BE-0023', '1', '14.90'),
  ('0018-V', 'BE-0056', '3', '17.90');


