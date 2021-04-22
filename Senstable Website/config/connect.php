<?php

include_once('config.php');

/*
 * file: connect.php
 *
 * This file opens a connection with the database
 * and will automatically create an object to work with.
 *
 */

// make sure the user can connect to the database via localhost

$pdo = new PDO("mysql:host=" . $app['db']['servername'] . ";dbname=" . $app['db']['database'], $app['db']['username'], $app['db']['password']);
