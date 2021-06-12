<?php

echo "1";
echo ",";
echo "0.85";

?>

<script>
    const serverSocket = "ws://145.24.222.125:8789";  // Change this to the right IP you're using for your server
    const raspberrySocket = "ws://192.168.4.1:5050";  // Change this if you're using a different IP for your Access Point

    const sensorJsonData = {
        "sensor": {
            "id": <?php echo $_GET["sensorId"] ?>,
            "value": <?php echo $_GET["sensorValue"] ?>,
            "physicalQuantity": '<?php echo $_GET["physicalQuantity"] ?>',
            "unit": '<?php echo $_GET["unit"] ?>'
        }
    };

    if ('<?php echo $_GET["outputId"] ?>' !== ''){
        outputJsonData = {
            "output": {
                "id": <?php echo $_GET["outputId"] ?>,
                "value": <?php echo $_GET["sensorValue"] ?>
            }
        };

        const socket = new WebSocket(raspberrySocket);
        socket.onopen = function(event) {
            socket.send(JSON.stringify(outputJsonData));
        };
    }

    const socket = new WebSocket(serverSocket);
    socket.onopen = function(event) {
        socket.send(JSON.stringify(sensorJsonData));
    };
</script>

<?php

require __DIR__ . '/vendor/autoload.php';

$sensorJsonData -> sensor -> id = (int)$_GET["sensorId"];
$sensorJsonData -> sensor -> value = (float)$_GET["sensorValue"];
$sensorJsonData -> sensor -> physicalQuantity = $_GET["physicalQuantity"];
$sensorJsonData -> sensor -> unit = $_GET["unit"];

\Ratchet\Client\connect("ws://145.24.222.125:8789")->then(function($conn) use ($sensorJsonData) {
    $conn->send(json_encode($sensorJsonData));
    $conn->close();
    header("Location: index.html");
});