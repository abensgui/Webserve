<?php
// Check if form was submitted
if(isset($_POST["submit"])) {
  // Check if file was uploaded without errors
  if(isset($_FILES["fileToUpload"]) && $_FILES["fileToUpload"]["error"] == 0) {
    $target_dir = "sup/";
    $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
    $imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
    
    // Allow certain file formats
    $allowed_types = array('jpg', 'jpeg', 'png', 'gif');
    if(in_array($imageFileType, $allowed_types)) {
      // Attempt to move the uploaded file to its new location
      if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        echo "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.";
      } else {
        echo "Sorry, there was an error uploading your file.";
      }
    } else {
      echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.";
    }
  } else {
    echo "Sorry, there was an error uploading your file.";
  }
}
else
{
  echo "Sorry, ayoub";
}
?>