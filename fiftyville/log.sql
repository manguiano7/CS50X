-- Keep a log of any SQL queries you execute as you solve the mystery.
-- identifies the theft of the CS50 duck at 10:15am at the Humphrey Street bakery and says there were 3 witnesses
SELECT description FROM crime_scene_reports WHERE year == 2021 AND month == 7 AND day == 28;

--identifies the three interviews from witnesses
SELECT transcript FROM interviews WHERE year == 2021 AND month == 7 AND day == 28 AND transcript LIKE "%bakery%";

--THE INTERSECT FINDS INTERSECTION OF PEOPLE FOUND FROM BAKERY AND PEOPLE WHO WITHDREW MONEY AND PEOPLE WHO
-- TOOK EARLY FLIGHT NEXT DAY
--identifies license plates of cars that left within 10 minutes of theft
SELECT people.name FROM bakery_security_logs JOIN people
    ON people.license_plate == bakery_security_logs.license_plate
        WHERE year == 2021 AND month == 7 AND day == 28
        AND hour == 10 AND minute >= 14 AND minute <= 26 AND activity = "exit"
INTERSECT
--identifies people who withdrew money on the same day and street the witness saw the thief
SELECT people.name FROM atm_transactions JOIN bank_accounts
    ON atm_transactions.account_number == bank_accounts.account_number
    JOIN people ON people.id == bank_accounts.person_id
        WHERE year == 2021 AND month == 7 AND day == 28
        AND transaction_type == "withdraw" AND atm_location == "Leggett Street"

INTERSECT -- FINDS PEOPLE WHO CALLED THAT DAY
SELECT people.name FROM phone_calls JOIN people
    ON people.phone_number == phone_calls.caller
        WHERE year == 2021 AND month == 7 AND day == 28 AND duration <= 60

INTERSECT -- finds license plates of people who were on next early flight
SELECT people.name FROM airports JOIN flights
    ON airports.id == flights.origin_airport_id JOIN passengers
    ON passengers.flight_id == flights.id JOIN people
    ON people.passport_number == passengers.passport_number
        WHERE flights.year == 2021 AND flights.month == 7 AND flights.day == 29 AND flights.hour == 8;

--finds where Bruce flew to
SELECT airports.city FROM airports JOIN flights
    ON airports.id == flights.destination_airport_id JOIN passengers
    ON passengers.flight_id == flights.id JOIN people
    ON people.passport_number == passengers.passport_number
        WHERE flights.year == 2021 AND flights.month == 7 AND flights.day == 29 AND flights.hour == 8
        AND people.name == "Bruce";

-- finds Bruce's number
SELECT people.phone_number FROM phone_calls JOIN people
    ON people.phone_number == phone_calls.caller
        WHERE year == 2021 AND month == 7 AND day == 28 AND duration <= 60
        AND people.name == "Bruce";

-- finds accomplice - person Bruce called on July 28 2021
SELECT people.name FROM phone_calls JOIN people
    ON people.phone_number == phone_calls.receiver
    WHERE year == 2021 AND month == 7 AND day == 28 AND duration <= 60
    AND phone_calls.receiver == people.phone_number AND phone_calls.caller ==
    --Bruce is the caller
(SELECT people.phone_number FROM phone_calls JOIN people
    ON people.phone_number == phone_calls.caller
        WHERE year == 2021 AND month == 7 AND day == 28 AND duration <= 60
        AND people.name == "Bruce");
