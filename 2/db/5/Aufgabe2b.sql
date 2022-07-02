SELECT verlagname FROM buch WHERE a_nr NOT IN (SELECT a_nr FROM medienartikel)
