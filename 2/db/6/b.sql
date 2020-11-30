SELECT b.be_nr, p.vorname, p.name, SUM(m.anzahl) / COUNT(b.be_nr=m.be_nr) FROM bestellung b, bestellung_beinhaltet_medienartikel m, person p WHERE b.be_nr=m.be_nr AND b.p_nr=p.p_nr GROUP BY p.vorname
