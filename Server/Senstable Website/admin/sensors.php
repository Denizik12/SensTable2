<?php
	session_start();

	// check if the user has acces to the home page
	if(!isset($_SESSION['user_login'])){
		include("index.php");
		exit();
	}
?>
<!DOCTYPE html>
<html lang="nl">
  	<head>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
		<meta name="description" content="">
		<meta name="author" content="">
		<title>Dashboard | Senstable</title>
		<style><?php include 'css/dashboard.css'; ?></style>
		<style><?php include 'css/bootstrap.min.css'; ?></style>
		<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.5.0/css/all.css" integrity="sha384-B4dIYHKNBt8Bc12p+WXckhzcICo0wtJAoU8YZTY5qE0Id1GSseTk6S+L3BlXeVIU" crossorigin="anonymous">
  	</head>
    <body onload="loadSensorTable()">
    <nav class="navbar navbar-dark sticky-top bg-dark flex-md-nowrap p-0">
      <a class="navbar-brand col-sm-3 col-md-2 mr-0" href="dashboard.php">Senstable</a>
      <ul class="navbar-nav px-3">
        <li class="nav-item text-nowrap">
          <a class="nav-link" href="logout.php">Uitloggen</a>
        </li>
      </ul>
    </nav>

    <div class="container-fluid">
      <div class="row">
        <nav class="col-md-2 d-none d-md-block bg-light sidebar">
          <div class="sidebar-sticky">
            <ul class="nav flex-column">
              <li class="nav-item">
                <a class="nav-link" href="dashboard.php">
                  <span data-feather="home"></span>
                  Dashboard
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link active" href="sensors.php">
                  <span data-feather="file"></span>
                  Sensors <span class="sr-only">(current)</span>
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="code.php">
                  <span data-feather="code"></span>
                  Code
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="ontwerpen.php">
                  <span data-feather="droplet"></span>
                  Ontwerpen
                </a>
              </li>
            </ul>
          </div>
        </nav>

        <main role="main" class="col-md-9 ml-sm-auto col-lg-10 pt-3 px-4">
          <h2>Sensors</h2>
          <div class="table-responsive">
            <table class="table table-striped table-sm" id="sensorTable">
				<thead>
					<tr>
						<th scope="col">#</th>
						<th scope="col">Naam</th>
						<th scope="col">Beschrijving</th>
						<th scope="col">Serienummer</th>
						<th scope="col">Datum toegevoegd</th>
						<th scope="col"></th>
						<th scope="col"></th>
					</tr>
				</thead>
				<tbody id="table"></tbody>
			</table>
			<p class="lbl-status" id="lbl-status"></p>

			<!-- edit sensor entry form -->
			<div class="form-popup" id="edit-popup-form">
				<form method="post" enctype="multipart/form-data">
					<h4 class="form-content" id="edit-form-header">Sensor bewerken: #</h4>
					<hr>

                    <p class="form-content">Sensor naam:</p>
					<input type="text" class="form-input form-content" id="sens-name-edit" name="sensName" placeholder="Naam van de sensor" required>

                    <p class="form-content">Sensor serienummer:</p>
					<input type="text" class="form-input form-content" id="sens-serial_number-edit" name="sensSerialNumber" placeholder="Serienummer (bijv. HC-SR04)" required>
					<hr>

                    <p class="form-content">Korte sensor beschrijving:</p>
					<textarea class="form-input form-content" id="sens-short_description-edit" rows="2" name="sensShortDescription" placeholder="Korte beschrijving (max 100 karakters)" required></textarea>

                    <p class="form-content">Sensor beschrijving:</p>
                    <textarea name="sensWiki" class="form-input form-content" id="sens-wiki-edit" rows="5" placeholder="Beschrijving over de sensor" required></textarea>
					<br>
					<button type="button" name="btn_save" class="btn btn-primary mb-1 form-content" onclick="saveSensor()">Opslaan</button>
					<button type="button" name="cancel" class="btn btn-cancel mb-1 form-content" onclick="closeEditForm()">Annuleren</button>
				</form>
			</div>
		  <br />
        </main>
      </div>
    </div>

    <!-- JS
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
	<script>
		let token ='<?php echo $_SESSION["token"];?>';
		localStorage.setItem('token', token);
		<?php 
			include 'js/sensors.js';
		?>
	</script>						
    <!-- Icons -->
    <script src="https://unpkg.com/feather-icons/dist/feather.min.js" type="text/javascript"></script>
    <script>
      feather.replace()
    </script>
  </body>
</html>
