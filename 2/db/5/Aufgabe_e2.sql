SELECT SUM(anzahl * einzelpreis)/ CAST((SELECT COUNT(*) FROM bestellung) AS INT) FROM bestellung_beinhaltet_medienartikel
