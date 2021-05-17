<html>
<head>

</head>

<body>
<p id="value">nee</p>
<p id="test">nee</p>

<script>
    const value = document.getElementById("value");
    const value2 = document.getElementById("test");

    let sock = new WebSocket("ws://145.24.222.125:8789"); //replace this address with the one the node.js server prints out.

    // get the paramater given in the url
    let parts = window.location.search.substr(1).split("&");
    let get = {};
    for (let i = 0; i < parts.length; i++) {
        let temp = parts[i].split("=");
        get[decodeURIComponent(temp[0])] = decodeURIComponent(temp[1]);
    }
    const id = get['id'];

    const jsonData = {
        "client": {
            "id": id
        }
    }

    if (id === "8") {
        value.innerHTML = id;

        // this function is called when the socket is connected
        sock.onopen = function(event) {

            // send the id from the sensor to the socket
            sock.send(JSON.stringify(jsonData));
        }

        // this function is called on every messaged received from the server
        sock.onmessage = function(event) {
            console.log(event.data);

            // update the graph with the new data
            //     update(JSON.parse(event.data));

            value2.innerHTML = event.data;
        }
    }
</script>
</body>
</html>
