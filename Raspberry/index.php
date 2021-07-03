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