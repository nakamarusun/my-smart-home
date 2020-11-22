CREATE TABLE IF NOT EXISTS weather (
  get_time DATETIME PRIMARY KEY NOT NULL,
  temp FLOAT,
  hindex FLOAT, 
  humid TINYINT UNSIGNED,
  pressure INT UNSIGNED,
  light INT UNSIGNED,
  rain BOOLEAN
);

CREATE TABLE IF NOT EXISTS door (
  event_time DATETIME PRIMARY KEY NOT NULL,
  event_type ENUM('proximity', 'doorbell')
);