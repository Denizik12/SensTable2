<script>
    const sensorJsonData = {
        "sensor": {
            "id": <?php echo $_GET["sensorId"] ?>,
            "value": <?php echo $_GET["sensorValue"] ?>,
            "physicalQuantity": '<?php echo $_GET["physicalQuantity"] ?>',
            "unit": '<?php echo $_GET["unit"] ?>'
        }
    };

    if ('<?php echo $_GET["outputId"] ?>' !== ''){
        window.alert("<?php echo $_GET["outputId"] ?>");
        outputJsonData = {
            "output": {
                "id": <?php echo $_GET["outputId"] ?>,
                "value": <?php echo $_GET["sensorValue"] ?>
            }
        };

        const socket = new WebSocket("ws://......");
        socket.onopen = function(event) {
            socket.send(JSON.stringify(outputJsonData));
        };
    }

    const socket = new WebSocket("ws://145.24.222.125:8789");
    socket.onopen = function(event) {
        socket.send(JSON.stringify(sensorJsonData));
    };

</script>