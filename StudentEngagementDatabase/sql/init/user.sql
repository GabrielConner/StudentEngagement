CREATE TABLE IF NOT EXISTS user (
    user_id INTEGER PRIMARY KEY,
    username VARCHAR(30),
    password VARCHAR(32),
    permission INTEGER
);