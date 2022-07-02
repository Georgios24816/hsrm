SELECT m.titel, AVG(a.preis) FROM medienartikel m, angebot a WHERE (m.a_nr=a.a_nr) GROUP BY a.a_nr
