CREATE TABLE IF NOT EXISTS user (
    user_id INT PRIMARY KEY,
    username VARCHAR(30),
    password VARCHAR(32),
    permission INT
);