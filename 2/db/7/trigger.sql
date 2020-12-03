CREATE TABLE anfangsgehalt (
  genre varchar(20) NOT NULL ,
  gehalt int(5) NOT NULL,
  PRIMARY KEY (genre)
);

INSERT INTO anfangsgehalt VALUES ('Drama', 100);

CREATE TABLE schauspieler (
  name varchar(20) NOT NULL,
  genre varchar(20) NOT NULL ,
  gehalt int(5) NULL,
  PRIMARY KEY (name)
);

CREATE TRIGGER trg_schauspieler1
  BEFORE INSERT ON schauspieler
  FOR EACH ROW
  SET NEW.gehalt = (SELECT gehalt FROM anfangsgehalt WHERE genre=NEW.genre);

INSERT INTO schauspieler (name, genre) VALUES ('Hans', 'Drama');

SELECT * FROM schauspieler;

CREATE TRIGGER trg_schauspieler2
  BEFORE UPDATE ON schauspieler
  FOR EACH ROW
  SET NEW.gehalt = IF(NEW.gehalt > 1.3 * OLD.gehalt, 1.3 * OLD.gehalt, NEW.gehalt);

UPDATE schauspieler SET gehalt='200' WHERE name='Hans';

SELECT * FROM schauspieler;

UPDATE schauspieler SET gehalt='150' WHERE name='Hans';

CREATE TABLE gehaltaenderung (
  name varchar(20) NOT NULL ,
  aenderung int(5) NOT NULL ,
  zeit timestamp NOT NULL
);

CREATE TRIGGER trg_schauspieler3
  AFTER UPDATE ON schauspieler
  FOR EACH ROW
  INSERT INTO gehaltaenderung (name, aenderung)
    VALUES (NEW.name, NEW.gehalt - OLD.gehalt);

UPDATE schauspieler SET gehalt='200' WHERE name='Hans';

SELECT * FROM gehaltaenderung;

SELECT * FROM schauspieler;

