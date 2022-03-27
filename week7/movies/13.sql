SELECT DISTINCT name
  FROM people
    JOIN stars 
      ON people.id = stars.person_id
    JOIN movies
      ON stars.movie_id = movies.id
    WHERE 
      movies.id IN
        (SELECT id 
          FROM movies
            JOIN stars
              ON movies.id = stars.movie_id
          WHERE person_id =
            (SELECT id
              FROM people
                WHERE name = "Kevin Bacon"
                  AND birth = 1958))
EXCEPT
SELECT name
  FROM people
    WHERE id =
      (SELECT id
        FROM people
          WHERE name = "Kevin Bacon"
            AND birth = 1958);