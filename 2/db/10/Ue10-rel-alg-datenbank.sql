CREATE TABLE studierende
       (matrnr         INTEGER PRIMARY KEY,
        name           VARCHAR(30) NOT NULL,
        semester       INTEGER
);

INSERT INTO studierende VALUES
(1401, 'Klaus', 10),
(1402, 'Michael', 3),
(1403, 'Kurt', 5),
(1404, 'Heidi', 4),
(1405, 'Nina', 5);

CREATE TABLE professor
       (pnr         INTEGER PRIMARY KEY,
        name        VARCHAR(30) NOT NULL,
        raum        VARCHAR(6) 
);

INSERT INTO professor VALUES
(1013, 'Fritz', 'I201'),
(8516, 'Maria', 'C1001'),
(1005, 'Julia', 'I111'),
(5313, 'Heinz', '1589');

CREATE TABLE vorlesung
       (vorlnr         INTEGER PRIMARY KEY,
        titel          VARCHAR(40),
        sws            INTEGER,
        gehaltenvon    INTEGER REFERENCES professor
);

INSERT INTO vorlesung VALUES
(107, 'Datenbanken', 4, 1013),
(286, 'Objektorientierte Softwareentwicklung', 6, 8516),
(456, 'Mathe', 4, 1005),
(117, 'Diskrete Strukturen', 4, 1013),
(118, 'Algorithmen und Datenstrukturen', 4, 1005);

CREATE TABLE setzt_voraus
       (vorgaenger     INTEGER REFERENCES vorlesung ON DELETE CASCADE,
        nachfolger     INTEGER REFERENCES vorlesung ON DELETE CASCADE,
        PRIMARY KEY    (vorgaenger, nachfolger));

INSERT INTO setzt_voraus VALUES
(286, 107),
(456, 286),
(117, 107),
(107, 118);

CREATE TABLE hoert
       (matrnr         INTEGER REFERENCES studierende ON DELETE CASCADE,
        vorlnr         INTEGER REFERENCES vorlesung ON DELETE CASCADE,
        PRIMARY KEY    (matrnr, vorlnr));

INSERT INTO hoert VALUES
(1401, 107),
(1402, 107),
(1401, 456),
(1401, 118);

