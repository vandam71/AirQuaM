CREATE DATABASE IF NOT EXISTS airquam;
USE aiquam;

CREATE TABLE IF NOT EXISTS Station(
    stationID INT NOT NULL AUTO_INCREMENT,
    stationName CHAR(20) NOT NULL,
    PRIMARY KEY (stationID)
);

CREATE TABLE IF NOT EXISTS Measurement(
    measurementID INT NOT NULL AUTO_INCREMENT,
    stationID INT NOT NULL,
    measurementTime TIME NOT NULL,
    measurementDate DATE NOT NULL,
    GPSlatitude FLOAT(3,8) NOT NULL,
    GPSlongitude FLOAT(3,8) NOT NULL,
    ZoneLatitude FLOAT(3,2) NOT NULL,
    ZoneLongitude FLOAT(3,2) NOT NULL,
    NO2 INT,
    CO INT,
    CO2 INT,
    TVOC INT,
    Temperature FLOAT(2,1),
    Humidity FLOAT(2,1),
    PRIMARY KEY (measurementID),
    FOREIGN KEY (stationID)
        REFERENCES Station(stationID)
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    FOREIGN KEY 
);

CREATE TABLE IF NOT EXISTS Zone(
    ZoneLatitude FLOAT(3,2) NOT NULL,
    ZoneLongitude FLOAT(3,2) NOT NULL
);

CREATE TABLE IF NOT EXISTS Prediction(
    ZoneLatitude FLOAT(3,2) NOT NULL,
    ZoneLongitude FLOAT(3,2) NOT NULL,
    predictionDate DATE NOT NULL,
    NO2 INT,
    CO INT,
    CO2 INT
    TVOC INT
);