<?php
include_once("match_core.php");
//
if(isset($_GET['url']) && trim($_GET['url']) != "") {
    $url = $_GET['url'];
}
//
$match_res = path_match($url);
if(empty($match_res['pattern'])) {
    $date = date("Y-m-d/H:i:s");
    $content = $date . "\t" . $match_res['url'] . "\n";
    $log_name = "./log/" . date("Y-m-d");
    error_log($content, 3, $log_name);
    echo json_encode($match_res);
}
else{
    echo json_encode($match_res);
}
?>
