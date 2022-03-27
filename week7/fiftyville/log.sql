-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.

SELECT description
  FROM crime_scene_reports
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND street = "Humphrey Street";

-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- Littering took place at 16:36. No known witnesses.

-- This query produces a rather long list of entries identified as entrance or exit, with exact times and license plate numbers. Clearly need more info before this is useful - an exact time, most likely.
-- SELECT activity, hour, minute, license_plate
--   FROM bakery_security_logs
--     WHERE year = 2021
--     AND month = 7
--     AND day = 28;

SELECT name, transcript
  FROM interviews
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND transcript 
      LIKE "%bakery%";

-- Ruth|
  -- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene|
  -- I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Raymond|
  -- As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
-- Emma|
  -- I'm the bakery owner, and someone came in, suspiciously whispering into a phone for about half an hour. They never bought anything.

-- Between 1015 and 1025, thief left bakery parking lot
-- Before EUGENE went to the bakery (time: check EUGENE license plate cross ref bakery)
    -- The theif WITHDREW money from ATM on LEGGETT STREET
-- Thief is taking first flight tomorrow (date?)
-- Thief had a one minute call as they left baker (between 1015 and 1025)

-- Find the plate numbers of everyone who left the baker in Ruth's specified time window
SELECT license_plate, minute
  FROM bakery_security_logs
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute > 15
    AND minute <= 25
    AND activity = 'exit';

-- 5P2BI95|16
-- 94KL13X|18
-- 6P58WS2|18
-- 4328GD8|19
-- G412CB7|20
-- L93JTIZ|21
-- 322W7JE|23
-- 0NTHK55|23

-- Look up EUGENE's plate number
SELECT license_plate, name 
  FROM people
    WHERE name 
      LIKE "%eugene%";
-- 47592FJ|Eugene

-- What TIME did EUGENE ARRIVE at the bakery
SELECT hour, minute, activity, day
  FROM bakery_security_logs
    WHERE year = 2021
    AND month = 7
    AND license_plate
    IN (
      SELECT license_plate
        FROM people
          WHERE name 
            LIKE "%eugene%"
    );

-- 13|22|entrance|26
-- 17|27|exit|26
-- 8|53|entrance|30
-- 15|45|exit|30

-- Apparently it's the 30th? 26th is pre-theft, but then 30th is the only other day that could be "earlier today"
-- Eugene not present at time of theft? Something weird going on with this lead

-- So what ACCOUNT_NUMBERS had a WITHDRAW before 8.53 on the 7.30.2021 at LEGGETT STREET
-- SELECT account_number, atm_location
--   FROM atm_transactions
--     WHERE year = 2021
--     AND month = 7
--     AND day = 30
--     AND (
--       (
--         hour < 8
--       )
--       OR
--       (
--         hour = 8
--         AND minute < 53
--       )
--     );

-- LOL transactions don't have timestamps. ok... 
-- so all WITHDRAW from LEGGETT on the 28th..
SELECT DISTINCT account_number
  FROM atm_transactions
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND transaction_type = 'withdraw'
    AND atm_location 
      LIKE '%Leggett%';

-- Need to scrap Eugene lead because of inconsistencies. 
-- Get the local airport
SELECT abbreviation, full_name, id
  FROM airports
  WHERE city = "Fiftyville";

-- Look up earliest flight from Fiftyville airport on the 29th
SELECT abbreviation, city, hour, minute, flights.id
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
  LIMIT 1;

-- LGA|New York City|8|20|36
-- They flew to New York City!!!

-- Look up passenger list on identified flight.
SELECT name
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
      );

-- One of these is the thief
-- Doris
-- Sofia
-- Bruce
-- Edward
-- Kelsey
-- Taylor
-- Kenny
-- Luca
    

-- We now have two sets to compare.
-- The plate numbers who left in Ruth's window (get people)
-- The people on the escape flight identified by Raymond.
-- and I guess let's re-add list of ATM WITHDRAWS LEGGET 28th

-- the person_ids of the folksies who withdrew
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
    );

-- the person_ids of the folksies who left bakery
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
    );

-- the person_ids of the people on the flight
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
      );

-- Find the NAME that appears in all three of these id lists
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
      );

-- Damn. Luca and Bruce. two names. Need another factor.

-- OK, so last piece might be from Raymond. Ids of callers from calls between 10.15 and 10.25 (when they were leaving the bakery) that lasted less than one minute. On another look, time is not available again.
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
      );

-- Add this to the mega-query as another cross ref!
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

-- Bruce you low-down dirty bastard.
-- Now let's just see who Bruce made that phone call too, eh.

SELECT name
  FROM people
    WHERE phone_number 
      IN (
        SELECT receiver
          FROM phone_calls
            WHERE id
              IN (
                SELECT id 
                  FROM phone_calls
                    WHERE year = 2021
                    AND month = 7
                    AND day = 28 
                    AND duration < 60
                    AND caller 
                      IN (
                        SELECT phone_number 
                          FROM people
                            WHERE name = "Bruce"
                      )
              )
      );

-- Robin. Lols batmen.