SELECT name
  FROM people
    WHERE id 
      IN (
        SELECT person_id
          FROM bank_accounts
            WHERE account_number
            IN (
              SELECT DISTINCT account_number
                FROM atm_transactions
                  WHERE year = 2021
                  AND month = 7
                  AND day = 28
                  AND transaction_type = 'withdraw'
                  AND atm_location 
                    LIKE '%Leggett%'
            )
      ) 
      AND 
      id IN (
        SELECT id
          FROM people
            WHERE license_plate
            IN (
              SELECT license_plate
                FROM bakery_security_logs
                  WHERE year = 2021
                  AND month = 7
                  AND day = 28
                  AND hour = 10
                  AND minute > 15
                  AND minute <= 25
                  AND activity = 'exit'
            )
      ) AND 
      id IN (
        SELECT people.id
          FROM people
          JOIN passengers
            ON people.passport_number = passengers.passport_number
            WHERE flight_id
              IN (
                SELECT flights.id
                  FROM flights
                  JOIN airports
                    ON destination_airport_id = airports.id
                  WHERE origin_airport_id 
                    IN (
                      SELECT id
                      FROM airports
                      WHERE city = "Fiftyville"
                    )
                  AND day = 29
                  AND month = 7
                  AND year = 2021
                  ORDER BY hour, minute
                  LIMIT 1
              )
      ) AND
      id IN (
        SELECT id
          FROM people
            WHERE phone_number
              IN (
                SELECT caller
                  FROM phone_calls
                    WHERE year = 2021
                    AND month = 7
                    AND day = 28
                    AND duration < 60
              )
      );