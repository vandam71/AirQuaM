var express = require('express');
var router = express.Router();
var connection = require('../lib/db')

router.get('/', function(req, res) {
    var list = [];
    connection.query('SELECT * from Prediction', function(err, rows, fields) {
        if (err) {
            console.log(err.message);
        } else {
            for (var i = 0; i < rows.length; i++) {
                var prediction = {
                    'ZoneID': rows[i].ZoneID,
                    'time': rows[i].time,
                    'date': rows[i].date,
                    'NO2': rows[i].NO2,
                    'CO': rows[i].CO,
                    'CO2': rows[i].CO2,
                    'TVOC': rows[i].TVOC
                }
                list.push(prediction);
            }
            res.render('prediction', { "list": list });
        }
    })

    /*
    connection.query('SELECT DISTINCT ZoneID from Prediction', function(err, result) {
        if (err) {
            console.log(err.message);
        } else {

            console.log(result);
            res.render('prediction');
        }
    })*/
});

router.get('/:id', function(req, res) {
    var list = [];
    connection.query('SELECT * FROM Prediction WHERE ZoneID=' + parseInt(req.params.id), function(err, rows, fields) {
        if (err) {
            console.log(err.message);
        } else {
            for (var i = 0; i < rows.length; i++) {
                var prediction = {
                    'ZoneID': rows[i].ZoneID,
                    'time': rows[i].time,
                    'date': rows[i].date,
                    'NO2': rows[i].NO2,
                    'CO': rows[i].CO,
                    'CO2': rows[i].CO2,
                    'TVOC': rows[i].TVOC
                }
                list.push(prediction);
            }
            res.render('prediction', { "list": list });
        }
    })
})

router.post('/post', function(req, res) {
    connection.query("INSERT INTO Prediction (ZoneID, time, date, NO2, CO, CO2, TVOC) VALUES ('" + req.body.ZoneID + "', '" + req.body.time + "', '" + req.body.date + "', '" + req.body.NO2 + "', '" + req.body.CO + "', '" + req.body.CO2 + "', '" + req.body.TVOC + "')", function(err, result) {
        if (err) {
            console.log(err.message);
        }
        res.sendStatus(200);
    })
})

module.exports = router;