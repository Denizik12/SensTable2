<?php
//database informatie
$host = "localhost";
$dbUsername = "senstable";
$dbPassword = "Senstable2";
$dbName = "senstable";

$conn = new mysqli($host, $dbUsername, $dbPassword, $dbName);

if (mysqli_connect_error()) {
    die('Connect Error(' . mysqli_connect_errno() . ')' . mysqli_connect_error());
}
?>

<html>
<head>
    <title>Senstable | Stadslab Rotterdam</title>
</head>
<body>

<form method="POST" action="controller.php">

    <select type="text" name="sensors">
        <option value="" hidden>Select sensor</option>
        <?php
        $result = mysqli_query($conn, "SELECT * FROM sensoren");
        while ($sensorRow = $result->fetch_assoc()) {
            echo "<option value='" . $sensorRow['id'] . "'>" . $sensorRow['id'] . " | " . $sensorRow['sensorType'] . "</option>";
        }
        ?>
    </select>

    <select type="text" name="outputs">
        <option value="" hidden>Select output</option>
        <?php
        $result = mysqli_query($conn, "SELECT * FROM outputs");
        while ($sensorRow = $result->fetch_assoc()) {
            echo "<option value='" . $sensorRow['id'] . "'>" . $sensorRow['id'] . " | " . $sensorRow['outputType'] . "</option>";
        }
        ?>
    </select>

    <button type="submit">submit</button>
</form>


</body>

</html>

<?php
//database informatie
$host = "localhost";
$dbUsername = "senstable";
$dbPassword = "Senstable2";
$dbName = "senstable";

$conn = new mysqli($host, $dbUsername, $dbPassword, $dbName);

$sensor = mysqli_real_escape_string($conn, $_POST["sensors"]);
$output = mysqli_real_escape_string($conn, $_POST["outputs"]);

if (mysqli_connect_error()) {
    die('Connect Error(' . mysqli_connect_errno() . ')' . mysqli_connect_error());
} else {
    // Controleert of de sensor al verbonden is met een output
    $checkSensorQuery = "SELECT * FROM relations WHERE sensorId = ?";
    $checkSensor = $conn -> prepare($checkSensorQuery);
    $checkSensor -> bind_param('s', $sensor);
    $checkSensor -> execute();
    $rowSensor = $checkSensor -> get_result() ;

    if ($rowSensor -> num_rows == 1) {
        // sensor is al verbonden met andere output, deze gaan we eerst verwijderen
        $removeSensorQuery = "DELETE FROM relations WHERE sensorId = ?";
        $removeSensor = $conn -> prepare($removeSensorQuery);
        $removeSensor -> bind_param('s', $sensor);
        $removeSensor -> execute();
    }

    // Controleert of de output al verbonden is met een sensor
    $checkOutputQuery = "SELECT * FROM relations WHERE outputId = ?";
    $checkOutput = $conn -> prepare($checkOutputQuery);
    $checkOutput -> bind_param('s', $output);
    $checkOutput -> execute();
    $rowOutput = $checkOutput -> get_result() ;

    if ($rowOutput -> num_rows == 1) {
        // output is al verbonden met andere sensor, deze gaan we eerst verwijderen
        $removeSensorQuery = "DELETE FROM relations WHERE outputId = ?";
        $removeSensor = $conn -> prepare($removeSensorQuery);
        $removeSensor -> bind_param('s', $output);
        $removeSensor -> execute();
    }

    // sensor en output verbinden
    $connectSensorQuery = "INSERT INTO relations (sensorId, outputId) VALUES (?, ?);";
    $connectSensor = $conn -> prepare($connectSensorQuery);
    $connectSensor -> bind_param('ss', $sensor, $output);
    $connectSensor -> execute();

    header("location: index.php");
}

?>