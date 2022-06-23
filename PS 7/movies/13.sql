--.schema
--SELECT name,birth FROM people WHERE name = "Kevin Bacon" AND birth = 1958;
SELECT name FROM people JOIN stars ON stars.person_id = people.id WHERE movie_id IN(SELECT movie_id FROM people JOIN stars ON stars.person_id = people.id WHERE name = "Kevin Bacon" and birth = 1958) AND name != "Kevin Bacon";
