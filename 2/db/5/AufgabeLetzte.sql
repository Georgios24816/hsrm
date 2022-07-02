SELECT titel FROM medienartikel WHERE a_nr IN (SELECT a_nr FROM schauspieler_spielt_in_film WHERE p_nr IN (SELECT p_nr FROM schauspieler WHERE mgr='Cristine Dembrowski'))
