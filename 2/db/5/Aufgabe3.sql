SELECT id, dauer FROM musikstueck WHERE id in (SELECT id FROM tontraeger_enthaelt_musikstueck WHERE a_nr IN (SELECT a_nr FROM medienartikel WHERE titel="The Best of Michael Jackson"))
