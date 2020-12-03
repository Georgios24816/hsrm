CREATE TABLE angestellte (
  angnr MEDIUMINT(5) NOT NULL,
  name VARCHAR(50) NOT NULL,
  `adr` VARCHAR(50),
  beruf VARCHAR(20),
  PRIMARY KEY (angnr)
);

CREATE TABLE pilot (
  angnr MEDIUMINT(5) NOT NULL,
  std MEDIUMINT(5),
  liz VARCHAR(50),
  PRIMARY KEY (angnr),
  FOREIGN KEY (angnr) REFERENCES angestellte (angnr)
);