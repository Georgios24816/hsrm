SELECT m.titel, CAST(AVG(a.preis) AS DECIMAL(4, 2)) FROM medienartikel m, angebot a WHERE (m.a_nr=a.a_nr) GROUP BY a.a_nr
