var express = require('express');
var router = express.Router();
var connection = require('../lib/db')

router.get('/', function(req, res) {
    var zones

    connection.query('SELECT DISTINCT ZoneID from Prediction', function(err, result) {
        if (err) {
            console.log(err.message)
        }
        res.render('prediction');
    })
});

module.exports = router;