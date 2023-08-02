SELECT people.name FROM people JOIN stars ON stars.person_id == people.id JOIN movies ON movies.id == stars.movie_id
WHERE people.name IS NOT "Kevin Bacon" AND movies.title IN
(SELECT movies.title FROM people JOIN stars ON stars.person_id == people.id JOIN movies ON movies.id == stars.movie_id
WHERE people.name == "Kevin Bacon" AND people.birth == 1958);
