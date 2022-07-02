CREATE VIEW view_a AS
SELECT p.p_nr, p.vorname, p.name, k.adresse FROM person p, kunde k WHERE (p.p_nr=k.p_nr);

