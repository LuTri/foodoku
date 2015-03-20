DROP TABLE IF EXISTS `userdata`;
CREATE TABLE `userdata` (
	`user_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	`spielername` TEXT NOT NULL UNIQUE,
	`passwort` TEXT,
	`nachname` TEXT,
	`vorname` TEXT,
	`anmeldedatum` TEXT
);

DROP TABLE IF EXISTS `games`;
CREATE TABLE `games` (
	`game_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	`user` INTEGER NOT NULL,
	`schwierigkeit` INTEGER NOT NULL,
	`spielzeitpunkt` TEXT NOT NULL,
	`zeit` INTEGER NOT NULL,
	`angezeigte_hilfen` INTEGER NOT NULL,
	`gefuellte_felder` INTEGER NOT NULL,
	`abgeschlossen` INTEGER NOT NULL,
	`gewertet` INTEGER NOT NULL,
	FOREIGN KEY(user) REFERENCES userdata(user_id)
);