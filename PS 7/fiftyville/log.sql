-- Keep a log of any SQL queries you execute as you solve the mystery.

-- understand how the schema is connected
.schema
-- No result from the video hint
SELECT description,street FROM crime_scene_reports WHERE month = 7 AND day =28 AND street = "Chamberlin Street";
-- Looking for reports on this day. Find out the theft of the cs50 duck took place at 10:15am @ Humphrey Street bakery
SELECT description,street FROM crime_scene_reports WHERE month = 7 AND day =28;
--looking for interviews
.table
.schema interviews
--Found three transcripts from the witnesses Ruth, Eugen and Raymon
--Ruth said: Thief got after 10mins driving a car -> See security footage
--Eugene: ATM leggett Street thief withdrawing money
--Raymond: thief leving the bakery (after 10 mins?) told a guy by phone that will take the earliest flight out of fiftyville.
-- the person on the phone bought the ticket.
SELECT name, transcript FROM interviews WHERE month = 7 AND day = 28;
--review bakery_secury_logs table
.schema bakery_security_logs
--Acess logs after 10mins. Founf out 3 cars left the bakery in this time window L93JTIZ, 322W7JE, 0NTHK55.
-- WHERE L93JTIZ in 8:18 out 10:21 / 322W7JE in 8:36 out 10:23 / 0NTHK55 in 8:42 out 10:23.
SELECT bakery_security_logs.id,bakery_security_logs.license_plate,bakery_security_logs.hour,bakery_security_logs.minute,people.name FROM bakery_security_logs,people WHERE bakery_security_logs.license_plate = people.license_plate AND day = 28 AND month = 7 AND hour = 10 AND minute >= 15 AND minute <= 30;
--Looking for information of those license_plates in the people table
--suspects Vanessa, Bruce, Barry, Luca, Sofia, Iman, Diana and Kelsey
--look for the atm withdraw on the 07/28 @ Leggett Street AtM
SELECT people.name, bank_accounts.account_number,atm_transactions.transaction_type FROM people, bank_accounts, atm_transactions WHERE people.id = bank_accounts.person_id AND atm_transactions.account_number = bank_accounts.account_number AND atm_transactions.month = 7 AND atm_transactions.day = 28 AND atm_transactions.atm_location = "Leggett Street" AND atm_transactions.trasaction_type = "withdraw";
--Found out that Bruce, Diana, Iman, Luca  a went to the atm at Leggett Street on the date 07/28 and withdraw
--loking for the phonecalls in the time and phone duration less then 1 minute
.schema phone_calls;

-- TODO
SELECT people.name, people.phone_number, phone_calls.caller, phone_calls.receiver,phone_calls.day, phone_calls.month, phone_calls.duration FROM people,phone_calls WHERE (phone_calls.caller = people.phone_number OR phone_calls.receiver = people.phone_number) AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60 AND (people.name = "Diana" OR people.name = "Luca" OR people.name = "Bruce" OR people.name = "Iman") ORDER BY phone_calls.duration DESC;
--leaving only DIANA and BRUCE
--DIANA was talking to (725) 555-3243
--BRUCE was talking to (375) 555-8161
SELECT name,phone_number FROM people WHERE phone_number ="(375) 555-8161" OR phone_number = "(725) 555-3243";
--DIANA was talking to (725) 555-3243 -> Philip
--BRUCE was talking to (375) 555-8161 -> Robin
-------------------------------------------------------------------------------------------
SELECT * FROM flights,airports WHERE month = 7 AND day = 29 AND city LIKE "%fifty%" ORDER BY hour;
--

SELECT people.id,flights.id,passengers.passport_number,people.name,flights.month,flights.day,flights.hour,flights.minute,flights.origin_airport_id,flights.destination_airport_id, airports.full_name FROM passengers,airports,flights,people WHERE airports.id = flights.origin_airport_id AND passengers.passport_number = people.passport_number AND passengers.flight_id = flights.id AND flights.month = 7 AND flights.day >= 28 AND people.name = "Bruce";
