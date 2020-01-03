var express = require('express');
var router = express.Router();
var shell = require('shelljs')


router.post('/python', function(req, res){

    res.redirect('/');
})

module.exports = router;