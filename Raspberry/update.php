<?php
require __DIR__ . '/vendor/autoload.php';

//database informatie
$host = "localhost";
$dbUsername = "senstable";
$dbPassword = "Senstable2";
$dbName = "senstable";

// Creeert connectie met database
$conn = new mysqli($host, $dbUsername, $dbPassword, $dbName);

if (mysqli_connect_error()) {
    die('Connect Error(' . mysqli_connect_errno() . ')' . mysqli_connect_error());
} else {
    //connectie goed

    // queries uitvoeren
    $bestaatSensor = "SELECT * FROM sensoren WHERE id=?";
    $getSensor = $conn -> prepare($bestaatSensor);
    $getSensor -> bind_param('s', $_GET["sensorId"]);
    $getSensor -> execute();
    $sensorRow = $getSensor -> get_result();

    $bestaatOutput = "SELECT * FROM relations WHERE sensorId=?";
    $getOutput = $conn -> prepare($bestaatOutput);
    $getOutput -> bind_param('s', $_GET["sensorId"]);
    $getOutput -> execute();
    $outputRow = $getOutput -> get_result();
    $outputRow = $outputRow -> fetch_assoc();

    // sensor toevoegen aan database
    if ($sensorRow -> num_rows == 0){
        $registrerenQuery = "INSERT INTO sensoren (id, sensorType) VALUES (?, ?);";
        $registreren = $conn -> prepare($registrerenQuery);
        $registreren -> bind_param('ss', $_GET["sensorId"], $_GET["sensorType"]);
        $registreren -> execute();
    }
}

$sensorJsonData -> sensor -> id = $_GET["sensorId"];
$sensorJsonData -> sensor -> value = $_GET["sensorValue"];
$sensorJsonData -> sensor -> physicalQuantity = $_GET["physicalQuantity"];
$sensorJsonData -> sensor -> unit = $_GET["unit"];

\Ratchet\Client\connect("ws://145.24.222.125:8789")->then(function($conn) use ($sensorJsonData) {
    $conn->send(json_encode($sensorJsonData));
    $conn->close();
});

if ($outputRow['outputId'] != null){
    $outputJsonData -> output -> id = $outputRow['outputId'];
    $outputJsonData -> output -> value = $_GET["sensorValue"];

    \Ratchet\Client\connect("ws://192.168.4.1:5050")->then(function($conn) use ($outputJsonData) {
        $conn->send(json_encode($outputJsonData));
        $conn->close();
    });
}