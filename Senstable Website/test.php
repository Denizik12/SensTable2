

<?php


$sensorJsonData -> sensor -> id = 3;
$sensorJsonData -> sensor -> value = 322;
$sensorJsonData -> sensor -> physicalQuantity = "Distance";
$sensorJsonData -> sensor -> unit = "cm";


$client = new WebSocket\Client("ws://145.24.222.125:8789");
$client->text(json_encode($sensorJsonData));
echo $client->receive();
$client->close();

?>

<?php

require __DIR__ . '/vendor/autoload.php';

$sensorJsonData -> sensor -> id = (int)$_GET["sensorId"];
$sensorJsonData -> sensor -> value = (float)$_GET["sensorValue"];
$sensorJsonData -> sensor -> physicalQuantity = $_GET["physicalQuantity"];
$sensorJsonData -> sensor -> unit = $_GET["unit"];

\Ratchet\Client\connect("ws://145.24.222.125:8789")->then(function($conn) use ($sensorJsonData) {
    $conn->send(json_encode($sensorJsonData));
    $conn->close();
});
