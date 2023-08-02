SELECT movies.title, ratings.rating FROM movies INNER JOIN
ratings ON ratings.movie_id == movies.id WHERE ratings.rating IS NOT NULL AND movies.year == 2010
ORDER BY ratings.rating DESC, movies.title;
