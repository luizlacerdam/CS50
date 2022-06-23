SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE name = "Helena Bonham Carter"
INTERSECT
SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE name = "Johnny Depp";
--SELECT title FROM movies WHERE id IN(SELECT movie_id FROM stars WHERE person_id IN(SELECT id FROM people WHERE name = "Helena Bonham Carter"));
--SELECT COUNT(title) FROM movies, stars, people WHERE movies.id = stars.movie_id AND stars.person_id = people.id AND name = "Johnny Depp" ORDER BY title ASC;
--SELECT title FROM movies JOIN stars ON movies.id = stars.movie_id JOIN people ON stars.person_id = people.id WHERE name = "Helena Bonham Carter" AND name = "Johnny Depp";
--SELECT title FROM movies WHERE id IN(SELECT movie_id from stars WHERE person_id IN(SELECT id FROM people WHERE name = "Johnny Depp") AND person_id IN(SELECT id FROM people WHERE name = "Helena Bonham Carter"));