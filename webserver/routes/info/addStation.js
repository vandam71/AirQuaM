var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

router.get('/station/add', function(req, res) {
    res.render('addStation')
})

router.post('/station/add', function(req, res) {
    const name = req.body.stationName
    connection.query("INSERT INTO Station (name, sampleRate, state) VALUES ('" + req.body.stationName + "', '" + req.body.sampleRate + "', '" + req.body.state + "')", function(err, result) {
        if (err) {
            console.error(err);
        }
    });
    res.redirect('/info/station/')
})

module.exports = router;